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
    public static void insertData(Connection con, String data){
        JsonParser parser = new JsonParser() ;
        JsonObject object;
        try{
            object = parser.parse(data).getAsJsonObject();
            JsonArray macAddressArray = object.get("mac address info").getAsJsonArray();
            JsonObject macAddressObject = macAddressArray.get(0).getAsJsonObject();
            //double macID = macAddressObject.get("id").getAsDouble();
            String macAddress = macAddressObject.get("id").getAsString();

            deleteTable(con, "serverinfo", macAddress);
            deleteTable(con, "dockerinfo", macAddress);
            deleteTable(con, "cpuinfo", macAddress);

            JsonArray cpuArray = object.get("cpu info").getAsJsonArray();
            JsonArray memoryArray = object.get("memory info").getAsJsonArray();
            JsonArray diskArray = object.get("disk info").getAsJsonArray();
            JsonArray networkArray = object.get("network info").getAsJsonArray();
            JsonArray dockerArray = object.get("docker info").getAsJsonArray();

            //table "serverinfo" insert
            for(int i = 0; i < memoryArray.size(); ++i) {
                JsonObject diskObject = diskArray.get(i).getAsJsonObject();
                JsonObject memoryObject = memoryArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into serverinfo (id, diskAvailable , diskTotal, diskUsed, memoryAvailable, memoryTotal, memoryUsed, networkDownloadRate, networkUploadRate, netCardNumber)" + "values(?,?,?,?,?,?,?,?,?,?)");

                insertSql.setString(1, macAddress);
                insertSql.setString(2, diskObject.get("available").getAsString());
                insertSql.setString(3, diskObject.get("total").getAsString());
                insertSql.setString(4, diskObject.get("used").getAsString());
                insertSql.setString(5, memoryObject.get("available").getAsString());
                insertSql.setString(6, memoryObject.get("total").getAsString());
                insertSql.setString(7, memoryObject.get("used").getAsString());

                double downloadRate = 0;
                double uploadRate = 0;
                int networkCard = networkArray.size();

                for(int j = 0; j < networkCard; ++j){
                    JsonObject networkObject = networkArray.get(i).getAsJsonObject();
                    downloadRate += networkObject.get("download rate").getAsDouble();
                    uploadRate += networkObject.get("upload rate").getAsDouble();
                }

                insertSql.setString(8, String.valueOf(downloadRate / networkCard));
                insertSql.setString(9, String.valueOf(uploadRate / networkCard));
                insertSql.setString(10, String.valueOf(networkCard));

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert server info success");

            //table "dockerinfo" insert
            for(int i = 0; i < dockerArray.size(); ++i) {
                JsonObject dockerObject = dockerArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into dockerinfo (id, cpuUsage, memoryUsed, memoryLimit, memoryUsage, dockerId)" + "values(?,?,?,?,?,?)");

                insertSql.setString(1, macAddress);
                insertSql.setString(2, dockerObject.get("cpu").getAsString());
                insertSql.setString(3, dockerObject.get("memory").getAsString());
                insertSql.setString(4, dockerObject.get("memory usage").getAsString());
                insertSql.setString(5, dockerObject.get("memory limit").getAsString());
                insertSql.setString(6, dockerObject.get("id").getAsString());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert docker info success");

            for(int i = 0; i < cpuArray.size(); ++i) {
                JsonObject cpuObject = cpuArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into cpuinfo (id, cpuUsed, cpuMhz, cpuCores, cpuId)" + "values(?,?,?,?,?)");

                insertSql.setString(1, macAddress);
                insertSql.setString(2, cpuObject.get("cpu source").getAsString());
                insertSql.setString(3, cpuObject.get("cpu Mhz").getAsString());
                insertSql.setString(4, cpuObject.get("cpu Cores").getAsString());
                insertSql.setString(5, cpuObject.get("id").getAsString());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert cpu info success");

        }catch (JsonIOException e1) {
            e1.printStackTrace();
        }catch (JsonSyntaxException e1) {
            e1.printStackTrace();
        }catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void deleteTable(Connection con, String command, String id){
        try{
            PreparedStatement deleteSql = con.prepareStatement("delete from " + command + " where id = '" + id + "'");
            deleteSql.executeUpdate();
            deleteSql.close();
        }catch (SQLException e){
            e.printStackTrace();
        }

    }
}
