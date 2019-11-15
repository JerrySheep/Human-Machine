import com.google.gson.*;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class JsonDataToMysql {
    private static final String url = "jdbc:mysql://localhost:3306/node_infos";
    private static final String user = "root";
    private static final String password = "20191104";
    private static Connection con;
    static Connection getConnect(){
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            con = DriverManager.getConnection(url, user, password);
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return con;
    }

    //insert data to mysql database
    public static double insertData(Connection con, String data){
        double macId = 0;
        JsonParser parser = new JsonParser() ;
        JsonObject object;
        try{
            object = parser.parse(data).getAsJsonObject();
            JsonArray macAddressArray = object.get("mac address info").getAsJsonArray();
            JsonObject macAddressObject = macAddressArray.get(0).getAsJsonObject();
            //double macID = macAddressObject.get("id").getAsDouble();
            double macAddress = macAddressObject.get("id").getAsDouble();
            macId = macAddress;

            deleteTable(con, "serverinfo", macAddress);
            deleteTable(con, "dockerinfo", macAddress);
            deleteTable(con, "cpuinfo", macAddress);

            JsonArray cpuArray = object.get("cpu info").getAsJsonArray();
            JsonArray memoryArray = object.get("memory info").getAsJsonArray();
            JsonArray diskArray = object.get("disk info").getAsJsonArray();
            JsonArray networkArray = object.get("network info").getAsJsonArray();
            //JsonArray dockerArray = object.get("docker info").getAsJsonArray();
            JsonArray locationArray = object.get("location info").getAsJsonArray();
            JsonArray timestampArray = object.get("timestamp info").getAsJsonArray();

            //table "serverinfo" insert
            for(int i = 0; i < memoryArray.size(); ++i) {
                JsonObject diskObject = diskArray.get(i).getAsJsonObject();
                JsonObject memoryObject = memoryArray.get(i).getAsJsonObject();
                JsonObject locationObject = locationArray.get(i).getAsJsonObject();
                JsonObject timestampObject = timestampArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into serverinfo (id, diskAvailable , diskTotal, diskUsed, memoryAvailable, memoryTotal, memoryUsed, networkDownloadRate, networkUploadRate, netCardNumber, location, timestamp)" + "values(?,?,?,?,?,?,?,?,?,?,?,?)");

                insertSql.setDouble(1, macAddress);
                insertSql.setDouble(2, diskObject.get("available").getAsDouble());
                insertSql.setDouble(3, diskObject.get("total").getAsDouble());
                insertSql.setDouble(4, diskObject.get("used").getAsDouble());
                insertSql.setDouble(5, memoryObject.get("available").getAsDouble());
                insertSql.setDouble(6, memoryObject.get("total").getAsDouble());
                insertSql.setDouble(7, memoryObject.get("used").getAsDouble());

                double downloadRate = 0;
                double uploadRate = 0;
                int networkCard = networkArray.size();

                for(int j = 0; j < networkCard; ++j){
                    JsonObject networkObject = networkArray.get(j).getAsJsonObject();
                    downloadRate += networkObject.get("download rate").getAsDouble();
                    uploadRate += networkObject.get("upload rate").getAsDouble();
                }

                insertSql.setDouble(8, downloadRate / networkCard);
                insertSql.setDouble(9, uploadRate / networkCard);
                insertSql.setInt(10, networkCard);
                insertSql.setString(11, locationObject.get("address").getAsString());
                insertSql.setString(12, timestampObject.get("time").getAsString());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert server info success");

            //table "cpuinfo" insert
            for(int i = 0; i < cpuArray.size(); ++i) {
                JsonObject cpuObject = cpuArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into cpuinfo (id, cpuUsed, cpuMhz, cpuCores, cpuId)" + "values(?,?,?,?,?)");

                insertSql.setDouble(1, macAddress);
                insertSql.setDouble(2, cpuObject.get("cpu source").getAsDouble());
                insertSql.setDouble(3, cpuObject.get("cpu Mhz").getAsDouble());
                insertSql.setInt(4, cpuObject.get("cpu Cores").getAsInt());
                insertSql.setInt(5, cpuObject.get("id").getAsInt());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert cpu info success");

            if(object.has("docker info")){
                //table "dockerinfo" insert
                JsonArray dockerArray = object.get("docker info").getAsJsonArray();
                for(int i = 0; i < dockerArray.size(); ++i) {
                    JsonObject dockerObject = dockerArray.get(i).getAsJsonObject();

                    PreparedStatement insertSql = con.prepareStatement("insert into dockerinfo (id, cpuUsage, memoryUsed, memoryUsage, memoryLimit, dockerId)" + "values(?,?,?,?,?,?)");

                    insertSql.setDouble(1, macAddress);
                    insertSql.setDouble(2, dockerObject.get("cpu").getAsDouble());
                    insertSql.setDouble(3, dockerObject.get("memory").getAsDouble());
                    insertSql.setDouble(4, dockerObject.get("memory usage").getAsDouble());
                    insertSql.setDouble(5, dockerObject.get("memory limit").getAsDouble());
                    insertSql.setString(6, dockerObject.get("id").getAsString());

                    insertSql.executeUpdate();
                    insertSql.close();
                }
                System.out.println("insert docker info success");
            }

        }catch (JsonIOException e1) {
            e1.printStackTrace();
        }catch (JsonSyntaxException e1) {
            e1.printStackTrace();
        }catch (SQLException e) {
            e.printStackTrace();
        }

        return macId;
    }

    public static void deleteTable(Connection con, String command, double id){
        try{
            PreparedStatement deleteSql = con.prepareStatement("delete from " + command + " where id = " + id);
            deleteSql.executeUpdate();
            deleteSql.close();
        }catch (SQLException e){
            e.printStackTrace();
        }

    }
}
