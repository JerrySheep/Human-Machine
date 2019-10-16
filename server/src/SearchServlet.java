import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.Connection;


public class SearchServlet extends HttpServlet {

    private static final long serialVersionUID = -4012838481920999524L;

    @Override
    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String query = request.getParameter("query");

        String result = "welcome to my server. It's a POST request.";
        if (null != query && !query.trim().equals("")) {
            result = query + ", " + result;
        }

        Tools.printToJson(result, response);

        System.out.println(getIpAddress(request));
        //System.out.println(getBodyData(request));
        String data = getBodyData(request);
        jsonToMysql(data);
        //System.out.println(data);
    }

    @Override
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String query = request.getParameter("query");

        String result = "welcome to my server. It's a GET request.";
        if (null != query && !query.trim().equals("")) {
            result = query + ", " + result;
        }

        Tools.printToJson(result, response);

        System.out.println(getIpAddress(request));
        //System.out.println(getBodyData(request));
        String data = getBodyData(request);
        jsonToMysql(data);
        //System.out.println(data);
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
    public void jsonToMysql(String data){
        Connection con = JsonDataToMysql.getConnect();
        JsonDataToMysql.insertData(con, data);
    }
}