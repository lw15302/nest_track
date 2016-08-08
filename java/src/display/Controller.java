package display;

import connect.Connection;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.image.ImageView;

public class Controller {
    @FXML
    private Button connect;

    @FXML
    private Button start;

    @FXML
    private ImageView dataView;

    @FXML
    private MenuBar menu;

    @FXML
    private Menu file, edit, analyse, help, export;

    @FXML
    private MenuItem save, exit, mean, median, stDev, about;

    Connection connection;


    @FXML
    private void piConnection() {
        connection = new Connection();
    }


}
