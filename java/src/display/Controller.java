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
    private Button connect, start, find, setFileName;

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
    private TextField saveLocation, fileName;

    @FXML
    private CheckBox saveCheck;

    private Stage primaryStage;
    private Connection connection;


    @FXML
    private void piConnection() {
        connection = new Connection();
        if(!connection.open()) failedConnection();
        else successfulConnection();
    }


    @FXML
    private void startTracking() {

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
//        System.out.println("File path: " + f.getPath());
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
    }
}
