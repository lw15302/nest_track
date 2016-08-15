package display;

import connect.Connection;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Controller {
    @FXML
    private Button connect, start, find, setFileName, setIp;

    @FXML
    private ImageView dataView;

    @FXML
    private MenuBar menu;

    @FXML
    private Menu file, edit, analyse, help, export;

    @FXML
    private MenuItem save, exit, mean, median, stDev, about;

    @FXML
    private Text status;

    @FXML
    private TextField saveLocation, fileName, ip1, ip2, ip3, ip4;

    @FXML
    private CheckBox saveCheck;

    private Stage primaryStage;
    private Connection connection;
    private boolean connected;
    private boolean tracking;
    private String[] hostIp;


    @FXML
    private void piConnection() {
        getIp();
        if(!connected) {
            disableSave(true);
            connection.open(hostIp);
            checkConnectionStatus();
        }
        else {
            disableSave(false);
            connected = false;
        }

    }


    @FXML
    private void startTracking() {
        if(!tracking) {
            start.setText("Stop Tracking");
            connection.track(hostIp);
            disableSave(true);
            tracking = true;
            checkTrackingStatus();
        }
        else {
            start.setText("Start Tracking");
            disableSave(false);
            tracking = false;
            connect.setDisable(false);
            connection.stopTrack(hostIp);
        }
    }


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


    @FXML
    private void setSaveLocation() {
        FileChooser navigator = new FileChooser();
        navigator.setTitle("Set Save Location");
        File f = navigator.showOpenDialog(primaryStage);
        String saveDir = formatSaveLocation(f.getPath());
        saveLocation.setText(saveDir);





        //Can't pick current folder, have to navigate to lowest child





        if(f.getName() != null) {
            fileName.setText(f.getName());
        }
        else {
            fileName.setText("default: " + "\"" + setDefaultSave() + "\"");
        }
    }


    @FXML
    private void lockFileName() {
        if(setFileName.getText().equals("Ok")) {
            find.setDisable(true);
            saveLocation.setDisable(true);
            fileName.setDisable(true);


            //Get save location and filename. If the filename does not have an extension, then add .txt. If another
            //extension, then warn them.



            setFileName.setText("Change");
        }
        else if(setFileName.getText().equals("Change")){
            find.setDisable(false);
            saveLocation.setDisable(false);
            fileName.setDisable(false);
            setFileName.setText("Ok");
        }
    }


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


    private void failedConnection() {
        status.setVisible(true);
        status.setText("Could not detect Raspberry Pi device");
        status.setFill(Color.RED);
    }


    private void successfulConnection() {
        status.setVisible(true);
        status.setText("Connected to Raspberry Pi device");
        status.setFill(Color.GREEN);
        start.setDisable(false);
        connect.setDisable(true);
    }


    private String setDefaultSave() {
        DateFormat dateFormat = new SimpleDateFormat("yyyy:MM:dd_HH:mm:ss");
        Date date = new Date();
        String defaultFileName = dateFormat.format(date) + ".txt";
        return defaultFileName;
    }


    private String formatSaveLocation(String filePath) {
        int len = filePath.length();
        int slash = 0;

        for(int i = 0; i < len; i++) {
            if((filePath.charAt(i)) == '/') {
                slash = i;
            }
        }

        String saveDir = filePath.substring(0, slash);
        return saveDir;
    }


    public void init(Stage primaryStage) {
        this.primaryStage = primaryStage;
        connection = new Connection();
        setDefaultIp();
        connected = false;
        tracking = false;
    }

    private void disableSave(boolean disable) {
        saveLocation.setDisable(disable);
        setFileName.setDisable(disable);
        saveCheck.setDisable(disable);
        fileName.setDisable(disable);
        find.setDisable(disable);
    }


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


    private void checkTrackingStatus() {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        tracking = connection.getTrackingStatus();
    }


    private void setDefaultIp() {
        ip1.setText("127");
        ip2.setText("0");
        ip3.setText("0");
        ip4.setText("1");
    }


    private void getIp() {
        hostIp = new String[4];
        try {
            hostIp[0] = ip1.getText();
            hostIp[1] = ip2.getText();
            hostIp[2] = ip3.getText();
            hostIp[3] = ip4.getText();
        } catch (Exception e) {
            //ERROR MESSAGE IF INCORRECT FORMAT!!!!!!



            throw new Error("Number format exception");











        }
    }
}
