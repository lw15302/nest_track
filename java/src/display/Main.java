package display;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Rectangle2D;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.stage.Screen;
import javafx.stage.Stage;

public class Main extends Application {
    private double screenW, screenH;
    private Controller mainController;

    @Override
    public void start(Stage primaryStage) throws Exception{
        try {
            getScreenDims();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("sample.fxml"));

            GridPane root = (GridPane) loader.load();

            Scene scene = new Scene(root, screenW, screenH);

            primaryStage.setTitle("Nest Track Controller");
            primaryStage.setScene(scene);
            primaryStage.show();

            mainController = loader.getController();
            mainController.init(primaryStage);
        } catch (Exception e) {
            e.printStackTrace();

        }
    }



    private void getScreenDims() {
        Rectangle2D screenBounds = Screen.getPrimary().getVisualBounds();

        screenW = screenBounds.getWidth();
        screenH = screenBounds.getHeight();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
