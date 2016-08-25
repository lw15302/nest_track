package data;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Slider;

import java.util.ArrayList;
import java.util.List;

public class DataManager {
    private final int dataSize;
    private int count, lastCount;


    public DataManager() {
        count = 0;
        dataSize = 1000;
//        nestCount.setName("Count");
//        series = new XYChart.Series<Number, Number>();
//        series.setName("Nest Count");
    }


    public void setSeries(int[] data, XYChart.Series<Number, Number> series) {
        int len = data.length;
        for (int i = 0; i < len; i++) {
            if (data[i] < 0) {
                count -= 1;
                float plottable = convertData(data[i], Direction.IN);
                if(plottable < 0) return;
                series.getData().add(new XYChart.Data<Number, Number>(plottable, count));
                lastCount = count;
                System.out.println("flag2" + " count: " + count);
            }
            else if (data[i] > 0) {
                count += 1;
                float plottable = convertData(data[i], Direction.OUT);
                if (plottable < 0) return;
                series.getData().add(new XYChart.Data<Number, Number>(plottable, count));
                lastCount = count;
                System.out.println("flag3" + " count: " + count);
            }
            else {

            }
        }
    }


    private float convertData(float data, Direction d) {
        float converted = 0;
        converted = (data / 1000);
        switch(d) {
            case IN:
                return (converted *-1);
            case OUT:
                return converted;
            default:
                return -1;
        }
    }
}
