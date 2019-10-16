import org.apache.http.HttpEntity;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;

public class HttpClient {
    public static void main(String[] args) {
        String data = "{\n" +
                "   \"cpu info\" : [\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 99.010000000000005,\n" +
                "         \"id\" : 0\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 1\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 2\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 3\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 4\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 5\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 6\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 7\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 95.049999999999997,\n" +
                "         \"id\" : 8\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 9\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 10\n" +
                "      },\n" +
                "      {\n" +
                "         \"cpu Cores\" : 6,\n" +
                "         \"cpu Mhz\" : 2500.0120000000002,\n" +
                "         \"cpu source\" : 100,\n" +
                "         \"id\" : 11\n" +
                "      }\n" +
                "   ],\n" +
                "   \"disk info\" : [\n" +
                "      {\n" +
                "         \"Avaiable\" : 290528440,\n" +
                "         \"Total\" : 613914040,\n" +
                "         \"Used\" : 0.52676039140593689\n" +
                "      }\n" +
                "   ],\n" +
                "   \"memory info\" : [\n" +
                "      {\n" +
                "         \"Avaiable\" : 47188544,\n" +
                "         \"Total\" : 49294724,\n" +
                "         \"Used\" : 0.042726276345517178\n" +
                "      }\n" +
                "   ],\n" +
                "   \"network info\" : [\n" +
                "      {\n" +
                "         \"download rate\" : 8.6611328125,\n" +
                "         \"id\" : 0,\n" +
                "         \"upload rate\" : 10.3740234375\n" +
                "      },\n" +
                "      {\n" +
                "         \"download rate\" : 2.6044921875,\n" +
                "         \"id\" : 1,\n" +
                "         \"upload rate\" : 2.6044921875\n" +
                "      }\n" +
                "   ]\n" +
                "}\n";
        HttpClient.report("http://127.0.0.1:11111/search", data);
    }


    public static boolean report(String url, String data) {
        CloseableHttpClient httpClient = null;
        CloseableHttpResponse response = null;
        try {
            // 创建发送端
            httpClient = HttpClients.createDefault();
            // 设置request的超时时间
            RequestConfig requestConfig = RequestConfig.custom().setConnectionRequestTimeout(5000)
                    .setConnectionRequestTimeout(5000).setSocketTimeout(5000).build();
            // 创建post请求
            HttpPost httpPost = new HttpPost(url);
            httpPost.setConfig(requestConfig);
            // 设置数据体
            httpPost.setEntity(new StringEntity(data));
            // 发送请求
            System.out.println(httpPost);
            response = httpClient.execute(httpPost);

            if (response.getStatusLine().getStatusCode() != 200) {
                System.out.println("fail");
                return false;
            }
            System.out.println("send success");

            HttpEntity entity = response.getEntity();

            if (entity != null) {
                if (EntityUtils.toString(entity, "UTF-8").equals("ok")) {
                    return true;
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (response != null) {
                    response.close();
                }
            } catch (Exception e2) {
                e2.printStackTrace();
            }
            try {
                if (httpClient != null) {
                    httpClient.close();
                }
            } catch (Exception e2) {
                e2.printStackTrace();
            }
        }
        return false;
    }
}