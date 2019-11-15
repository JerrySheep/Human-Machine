import com.google.gson.*;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.sql.Connection;


public class SearchServlet extends HttpServlet {

    private static final long serialVersionUID = -4012838481920999524L;

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String query = request.getParameter("query");

        String result = "welcome to my server. It's a POST request.";

        System.out.println(getIpAddress(request));
        //System.out.println(getBodyData(request));
        String data = getBodyData(request);
        //System.out.println(data);
        double macId = jsonToMysql(data);

        result += "\n" + getDockerResponse(macId);

        if (null != query && !query.trim().equals("")) {
            result = query + ", " + result;
        }

        Tools.printToJson(result, response);
    }

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String query = request.getParameter("query");

        String result = "welcome to my server. It's a GET request.";

        System.out.println(getIpAddress(request));
        //System.out.println(getBodyData(request));
        String data = getBodyData(request);
        //System.out.println(data);
//        String macId = jsonToMysql(data);
//        System.out.println("macId: " + macId);
//
//        result += "\n" + getDockerResponse(macId);

        if (null != query && !query.trim().equals("")) {
            result = query + ", " + result;
        }

        Tools.printToJson(result, response);
    }

    // get data from the request
    public String getBodyData(HttpServletRequest request) {
        String bodyStr = null;
        BufferedReader br = null;

        try {
            request.setCharacterEncoding("UTF-8");
            br = new BufferedReader(new InputStreamReader(request.getInputStream()));
            // get data from body
            String line = "";
            StringBuffer buf = new StringBuffer();
            while ((line = br.readLine()) != null) {
                buf.append(line);
            }
            bodyStr = buf.toString();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return bodyStr;
    }

    private static String getIpAddress(HttpServletRequest request) {
        String ip = request.getHeader("X-Forwarded-For");

        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("Proxy-Client-IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("WL-Proxy-Client-IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("HTTP_CLIENT_IP");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getHeader("HTTP_X_FORWARDED_FOR");
        }
        if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
            ip = request.getRemoteAddr();
        }
        return ip;
    }

    //store the data to mysql
    public double jsonToMysql(String data){
        Connection con = JsonDataToMysql.getConnect();
        double macId = JsonDataToMysql.insertData(con, data);

        return macId;
    }

    public static String readJsonFile(String fileName) {
        String jsonStr = "";
        try {
            File jsonFile = new File(fileName);
            FileReader fileReader = new FileReader(jsonFile);

            Reader reader = new InputStreamReader(new FileInputStream(jsonFile),"utf-8");
            int ch = 0;
            StringBuffer sb = new StringBuffer();
            while ((ch = reader.read()) != -1) {
                sb.append((char) ch);
            }
            fileReader.close();
            reader.close();
            jsonStr = sb.toString();
            return jsonStr;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public String getDockerResponse(double macId){
        String result = "null";
        String dockerResponseJson = readJsonFile("/home/wangch/yanhao/dockerResponse.json");
        JsonParser parser = new JsonParser() ;
        JsonObject object;
        object = parser.parse(dockerResponseJson).getAsJsonObject();
        JsonArray dockerArray = object.get("docker response info").getAsJsonArray();

        for(int i = 0; i < dockerArray.size(); ++i) {
            JsonObject dockerObject = dockerArray.get(i).getAsJsonObject();
            int response = dockerObject.get("response").getAsInt();
            double serverId = dockerObject.get("server id").getAsDouble();

            if(serverId == macId && response == 1){
                result = dockerObject.get("docker address").getAsString();
                break;
            }
        }

        return result;
    }
}