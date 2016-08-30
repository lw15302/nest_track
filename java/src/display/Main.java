package display;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Rectangle2D;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

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

            primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
                @Override
                public void handle(WindowEvent event) {
                    mainController.stop();
                    primaryStage.close();
                    Platform.exit();
                }
            });
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
