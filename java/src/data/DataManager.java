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
    private int count, highestCount, total;
    private float mean;
    private float recentActivity;


    public DataManager() {
        count = 0;
        dataSize = 1000;
        recentActivity = 0;
        highestCount = 0;
        mean = 0;
        total = 0;
    }


    /**
     * Updates the data series
     * @param data the raw data in an array
     * @param series the data series to be updated
     */
    public void setSeries(int[] data, XYChart.Series<Number, Number> series) {
        int len = data.length;
        for (int i = 0; i < len; i++) {
            if (data[i] < 0) {
                count -= 1;
                float plottable = convertData(data[i], Direction.IN);
                if(plottable < 0) return;
                recentActivity = plottable;
                calculateMean(series);
                if(count > highestCount) highestCount = count;
                series.getData().add(new XYChart.Data<Number, Number>(plottable, count));
            }
            else if (data[i] > 0) {
                count += 1;
                float plottable = convertData(data[i], Direction.OUT);
                if (plottable < 0) return;
                recentActivity = plottable;
                calculateMean(series);
                if(count > highestCount) highestCount = count;
                series.getData().add(new XYChart.Data<Number, Number>(plottable, count));
            }
            else {

            }
        }
    }


    public void reset() {
        recentActivity = 0;
        count = 0;
        highestCount = 0;
        mean = 0;
        total = 0;
    }

    public int getCount() {
        return count;
    }


    public float getRecentActivity() {
        return recentActivity;
    }


    public int getHighestCount() {
        return highestCount;
    }


    public float getMean() {
        return mean;
    }


    /**
     * Convert data to seconds and make positive if it is negative
     * @param data data value
     * @param d indicates whether or not to multiply by -1
     * @return
     */
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


    private void calculateMean(XYChart.Series<Number, Number> series) {
        total += count;
        mean = (float)total / series.getData().size();
    }
}
