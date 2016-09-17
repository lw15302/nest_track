package display;

import connect.Connection;

import data.DataManager;
import data.SpreadSheetHandler;
import data.Table;
import data.TextDataHandler;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.application.Platform;

import javafx.fxml.Initializable;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;

import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;

import java.io.IOException;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.ResourceBundle;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

public class Controller implements Initializable {
    @FXML
    private Button connect, start, find, setFileName, setIp;

    @FXML
    private MenuBar menu;

    @FXML
    private Menu file, edit, analyse, help, export;

    @FXML
    private MenuItem save, exit, mean, median, stDev, about;

    @FXML
    private Text status, trackingStatus;

    @FXML
    private Text statsCount, statsHighCount, statsMean,statsRecent;

    @FXML
    private TextField saveLocation, fileName, ip1, ip2, ip3, ip4;

    @FXML
    private CheckBox saveCheck;

    @FXML
    private LineChart<Number, Number> chart;

    @FXML
    private TableView dataTable;

    @FXML
    private TableColumn<Table, Float> timeCol;

    @FXML
    private TableColumn<Table, Integer> countCol;

    @FXML
    private NumberAxis xAxis, yAxis;

    private XYChart.Series<Number, Number> series;
    private Stage primaryStage;
    private Connection connection;
    private DataManager dm;
    private TextDataHandler textData;
    private SpreadSheetHandler spreadSheetData;
    private ScheduledExecutorService executor;
    private ScheduledFuture future;
    private ObservableList<Table> dataForTable;

    private String previousFileName;
    private boolean connected;
    private boolean tracking;
    private String[] hostIp;
    private int[] data;
    private int countForTable;


    /**
     * Sends a signal to the address specified. If there is a server at the specified address then it returns a signal.
     * The GUI can then react accordingly
     */
    @FXML
    private void piConnection() {
        getIp();
        if(!connected) {
            connection.open(hostIp);
            checkConnectionStatus();
        }
        else {
            connected = false;
        }

    }


    /**
     * Contains the logic to start/stop and maintain tracking. Also updates the live chart.
     */
    @FXML
    private void startTracking() {
        if(!tracking) {
            trackingStatus.setVisible(true);
            fillEmptySaveLocation();
            previousFileName = fileName.getText();

            clearData();
            start.setText("Stop Tracking");
            disable(true);

            series.getData().add(new XYChart.Data<Number, Number>((float)0.0, 0));

            connection.track(hostIp);
            checkTrackingStatus();
            if(!tracking) {
                connection.stopTrack(hostIp);
                return;
            }

            Runnable dataGetter = () -> {

                while (tracking) {
                    data = connection.getData(hostIp);
                    for(int i = 0; i < 10; i++) {
                        System.out.print(data[i] + " ");
                    }
                    System.out.println();
                    if (trackingStopped(data)) {
                        break;
                    }
                    addToTable();

                    Platform.runLater(() -> {
                        dm.setSeries(data, series);
                        System.out.println("Series: " + series.getData());
                        chart.getData().retainAll();
                        chart.getData().addAll(series);
                        updateStats();
                    });
                }
            };
            future = executor.scheduleWithFixedDelay(dataGetter, 0, 10, TimeUnit.MILLISECONDS);
        }
        else if(tracking){
            trackingStatus.setVisible(false);
            start.setText("Start Tracking");
            future.cancel(true);
            future = null;
            disable(false);
            tracking = false;
            connect.setDisable(false);
            connection.stopTrack(hostIp);
            if(saveCheck.isSelected()) saveText();
        }
    }


    /**
     * Enable/disable save options
     */
    @FXML
    private void setSavePreferences() {
        if(saveCheck.isSelected()) {
            saveLocation.setDisable(false);
            fileName.setDisable(false);
            find.setDisable(false);
            setFileName.setDisable(false);
        }
        else {
            saveLocation.setDisable(true);
            fileName.setDisable(true);
            find.setDisable(true);
            setFileName.setDisable(true);
        }
    }


    /**
     * Opens a file chooser to allow the user to graphically choose save location
     */
    @FXML
    private void setSaveLocation() {
        FileChooser navigator = new FileChooser();
        navigator.setTitle("Set Save Location");
        File f = navigator.showOpenDialog(primaryStage);
        String toSave = f.getPath();
        System.out.println("\nf.getpath(): " + toSave);
        String saveDir = formatSaveLocation(toSave, SavePath.DIRECTORYNAME);
        String file = formatSaveLocation(toSave, SavePath.FILENAME);

        saveLocation.setText(saveDir);
        fileName.setText(file);
    }


    /**
     * Enable/disable file name
     */
    @FXML
    private void lockFileName() {
        if(setFileName.getText().equals("Ok")) {
            find.setDisable(true);
            saveLocation.setDisable(true);
            fileName.setDisable(true);
            setFileName.setText("Change");
        }
        else if(setFileName.getText().equals("Change")){
            find.setDisable(false);
            saveLocation.setDisable(false);
            fileName.setDisable(false);
            setFileName.setText("Ok");
        }
    }


    /**
     * Enable/disable IP choice
     */
    @FXML
    private void lockIp() {
        if(setIp.getText().equals("Ok")) {
            ip1.setDisable(true);
            ip2.setDisable(true);
            ip3.setDisable(true);
            ip4.setDisable(true);
            setIp.setText("Change");
        }
        else {
            ip1.setDisable(false);
            ip2.setDisable(false);
            ip3.setDisable(false);
            ip4.setDisable(false);
            setIp.setText("Ok");
        }
    }


    /**
     * Called if no server detected. Displays red error message
     */
    private void failedConnection() {
        status.setVisible(true);
        status.setText("Could not detect Raspberry Pi device");
        status.setFill(Color.RED);
    }


    /**
     * Called if server is available. Displays green message
     */
    private void successfulConnection() {
        status.setVisible(true);
        status.setText("Connected to Raspberry Pi device");
        status.setFill(Color.GREEN);
        start.setDisable(false);
        connect.setDisable(true);
    }


    /**
     * Sets default file name to date and time if none is chosen
     * @return default file name
     */
    private String setDefaultFileName() {
        DateFormat dateFormat = new SimpleDateFormat("yyyy:MM:dd_HH:mm:ss");
        Date date = new Date();
        String defaultFileName = dateFormat.format(date) + ".txt";
        return defaultFileName;
    }


    /**
     * Ensures that file name or directory name are in the correct format
     * @param filePath The file name/directory name entered
     * @param s file name or directory name
     * @return The formatted file name or directory name
     */
    private String formatSaveLocation(String filePath, SavePath s) {
        int len = filePath.length();
        int slash = 0;

        for(int i = 0; i < len; i++) {
            if((filePath.charAt(i)) == '/') {
                slash = i;
            }
        }

        switch(s) {
            case FILENAME:
                String save = filePath.substring(slash + 1, len);
                if(!save.endsWith(".txt")) return save + ".txt";
                return save;
            case DIRECTORYNAME:
                return filePath.substring(0, slash + 1);
            default:
                return "";
        }
    }


    public void init(Stage primaryStage) {
        this.primaryStage = primaryStage;
        connection = new Connection();
        dm = new DataManager();
        executor = Executors.newSingleThreadScheduledExecutor();

        resetStats();

        data = new int[1000];
        dataForTable = FXCollections.observableArrayList();
        countForTable = 0;

        spreadSheetData = new SpreadSheetHandler();

        xAxis.setAutoRanging(true);
        yAxis.setAutoRanging(true);

        series = new XYChart.Series<>();
        series.setName("Nest count");
        chart.setLegendVisible(false);
        chart.setAnimated(false);

        chart.getData().addAll(series);

        setDefaultIp();
        connected = false;
        tracking = false;
    }


    /**
     * Enables or disables the user selectable options
     * @param toDisable enable or disable
     */
    private void disable(boolean toDisable) {
        ip1.setDisable(toDisable);
        ip2.setDisable(toDisable);
        ip3.setDisable(toDisable);
        ip4.setDisable(toDisable);
        setIp.setDisable(toDisable);

        saveLocation.setDisable(toDisable);
        setFileName.setDisable(toDisable);
        saveCheck.setDisable(toDisable);
        fileName.setDisable(toDisable);
        find.setDisable(toDisable);
    }


    /**
     * Check if a connection is possible
     */
    private void checkConnectionStatus() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        connected = connection.getConnectionStatus();
        if(connected) successfulConnection();
        else failedConnection();
    }


    /**
     * Check if tracking is running
     */
    private void checkTrackingStatus() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        tracking = connection.getTrackingStatus();
    }


    /**
     * Sets IP boxes to default values
     */
    private void setDefaultIp() {
        ip1.setText("127");
        ip2.setText("0");
        ip3.setText("0");
        ip4.setText("1");
    }


    /**
     * Get the IP from the IP boxes
     */
    private void getIp() {
        hostIp = new String[4];
        try {
            hostIp[0] = ip1.getText();
            hostIp[1] = ip2.getText();
            hostIp[2] = ip3.getText();
            hostIp[3] = ip4.getText();
        } catch (Exception e) {
            throw new Error("Number format exception");
        }
    }


    /**
     * Steps taken when tracking is stopped
     * @param data the data array
     * @return true if tracking has stopped,  else false
     */
    private boolean trackingStopped(int[] data) {
        int count = 0;
        int len = data.length;
        for(int i = 0; i < len; i++) {
            if(data[i] == -1) count++;
            if(count > 4) {
                Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        trackingStatus.setVisible(false);
                        future.cancel(true);
                        future = null;
                        tracking = false;
                        disable(false);
                        connect.setDisable(false);
                        start.setText("Start Tracking");
                        if(saveCheck.isSelected()) saveText();
                    }
                });
                return true;
            }
        }
        return false;
    }


    /**
     * Save the results to a text file
     */
    private void saveText() {
        try {
            List<Object> timeData = new ArrayList<>();
            List<Object> countData = new ArrayList<>();

            String savePlace = saveLocation.getText();
            String file = fileName.getText();

            textData = new TextDataHandler(savePlace, file);

            dataToSave(timeData, DataType.TIME);

            dataToSave(countData, DataType.COUNT);

            textData.save(timeData, countData);
        } catch (IOException e) {
            System.err.println("\nUnable to save to this location");

            //Handle with on screen warning
        }
    }


    /**
     * Convert data series to a list, ready to save
     * @param data a list to fill with data
     * @param type time or count
     */
    private void dataToSave(List<Object> data, DataType type) {
        switch(type) {
            case TIME:
                for(XYChart.Data xy : series.getData()) {
                    data.add(xy.getXValue());
                }
                break;
            case COUNT:
                for(XYChart.Data xy : series.getData()) {
                    data.add( xy.getYValue());
                }
                break;
            default:
                return;
        }

    }


    /**
     * Fills save location with default values if the user doesn't enter one
     */
    private void fillEmptySaveLocation() {
        if(saveCheck.isSelected()) {
            if(saveLocation.getText().equals("")) {
                saveLocation.setText(System.getProperty("user.dir") + "/data/text" + "/");
            }
            if(fileName.getText().equals("") || fileName.getText().equals(previousFileName)) {
                fileName.setText(setDefaultFileName());
            }
        }
    }


    /**
     * Clears data series, chart and table at the end of the experiment
      */
    private void clearData() {
        chart.getData().removeAll(series);
        series.getData().clear();
        dm.reset();
        if(!dataForTable.isEmpty()) {
            dataForTable.clear();
        }
        dataTable.setItems(dataForTable);
        countForTable = 0;
    }


    /**
     * Add data to table
     */
    private void addToTable() {
        int len = data.length;
        for(int i = 0; i < len && data[i] != 0; i++) {
            if(data[i] < 0) {
                float timeForTable;
                countForTable--;
                timeForTable = data[i] * -1;
                Table t = new Table(countForTable, timeForTable/1000);
                dataForTable.add(t);
            }
            else {
                countForTable++;
                Table t = new Table(countForTable, (float)data[i]/1000);
                dataForTable.add(t);
            }
            System.out.println("inside addToTable loop, i: " + i);
        }
    }


    /**
     * Reset the stats box
     */
    private void resetStats() {
        statsCount.setText("0");
        statsHighCount.setText("0");
        statsMean.setText("0.0");
        statsRecent.setText("0.0 seconds");
    }


    /**
     * Update stats with new values
     */
    private void updateStats() {
        statsCount.setText(Integer.toString(dm.getCount()));
        statsRecent.setText(Float.toString(dm.getRecentActivity()) + " seconds");
        statsHighCount.setText(Integer.toString(dm.getHighestCount()));
        statsMean.setText(Float.toString(dm.getMean()));
    }


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        countCol.setCellValueFactory(new PropertyValueFactory<Table, Integer>("count"));
        timeCol.setCellValueFactory(new PropertyValueFactory<Table, Float>("time"));

        dataTable.setItems(dataForTable);
    }
}
