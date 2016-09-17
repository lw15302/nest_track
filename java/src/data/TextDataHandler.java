package data;

import java.io.*;
import java.util.List;

public class TextDataHandler {
    private String saveLocation, fileName;


    public TextDataHandler(String saveLocation, String fileName) throws IOException {
        this.saveLocation = saveLocation;
        this.fileName = fileName;
    }

    /**
     * Save data to text file
     * @param timeData time data
     * @param countData count data
     * @throws IOException throws exception if save is not possible
     */
    public void save(List<Object> timeData, List<Object> countData) throws IOException {
        try ( PrintWriter out = new PrintWriter(saveLocation + fileName)) {
            int length = timeData.size();

            for(int i = 0; i < length; i++) {
                String time = Float.toString((Float)timeData.get(i));
                int count = (int)countData.get(i);

                out.print(time);
                out.print(",");
                out.println(count);
            }
        }
    }
}
