import com.google.gson.*;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class JsonDataToMysql {
    private static final String url = "jdbc:mysql://localhost:36820/linux_resource";
    private static final String user = "root";
    private static final String password = "";
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

            deleteTable(con, "cpu");
            deleteTable(con, "disk");
            deleteTable(con, "memory");
            deleteTable(con, "network");

            JsonArray cpuArray = object.get("cpu info").getAsJsonArray();
            for(int i = 0; i < cpuArray.size(); ++i) {
                JsonObject arrayObject = cpuArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into cpu (id, resource, MHz , cores)" + "values(?,?,?,?)");

                insertSql.setInt(1, arrayObject.get("id").getAsInt());
                insertSql.setDouble(2, arrayObject.get("cpu source").getAsDouble());
                insertSql.setDouble(3, arrayObject.get("cpu Mhz").getAsDouble());
                insertSql.setInt(4, arrayObject.get("cpu Cores").getAsInt());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert cpu info success");

            JsonArray memoryArray = object.get("memory info").getAsJsonArray();
            for(int i = 0; i < memoryArray.size(); ++i) {
                JsonObject arrayObject = memoryArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into memory (total, available , used)" + "values(?,?,?)");

                insertSql.setDouble(1, arrayObject.get("Total").getAsDouble());
                insertSql.setDouble(2, arrayObject.get("Available").getAsDouble());
                insertSql.setDouble(3, arrayObject.get("Used").getAsDouble());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert memory info success");

            JsonArray diskArray = object.get("disk info").getAsJsonArray();
            for(int i = 0; i < diskArray.size(); ++i) {
                JsonObject arrayObject = diskArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into disk (total, available , used)" + "values(?,?,?)");

                insertSql.setDouble(1, arrayObject.get("Total").getAsDouble());
                insertSql.setDouble(2, arrayObject.get("Available").getAsDouble());
                insertSql.setDouble(3, arrayObject.get("Used").getAsDouble());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert disk info success");

            JsonArray networkArray = object.get("network info").getAsJsonArray();
            for(int i = 0; i < networkArray.size(); ++i) {
                JsonObject arrayObject = networkArray.get(i).getAsJsonObject();

                PreparedStatement insertSql = con.prepareStatement("insert into network (id, uploadRate , downloadRate)" + "values(?,?,?)");

                insertSql.setInt(1, arrayObject.get("id").getAsInt());
                insertSql.setDouble(2, arrayObject.get("upload rate").getAsDouble());
                insertSql.setDouble(3, arrayObject.get("download rate").getAsDouble());

                insertSql.executeUpdate();
                insertSql.close();
            }
            System.out.println("insert network info success");

        }catch (JsonIOException e1) {
            e1.printStackTrace();
        }catch (JsonSyntaxException e1) {
            e1.printStackTrace();
        }catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void deleteTable(Connection con, String command){
        try{
            PreparedStatement deleteSql = con.prepareStatement("delete from " + command);
            deleteSql.executeUpdate();
            deleteSql.close();
        }catch (SQLException e){
            e.printStackTrace();
        }

    }
}
