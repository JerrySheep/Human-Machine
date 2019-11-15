import org.eclipse.jetty.server.Connector;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.ServerConnector;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

public class JettyServletExampleServer {

    public static void main(String[] args) {
        try {
            Server server = new Server(8080);
//            ServerConnector connector = new ServerConnector(server);
//            connector.setPort(8080);
//            server.setConnectors(new Connector[] { connector });

//            ServerConnector connector = new ServerConnector(server);
//            connector.setPort(11111);
//            connector.setHost("localhost");
//            server.setConnectors(new Connector[] { connector });

            ServletContextHandler context = new ServletContextHandler(
                    ServletContextHandler.SESSIONS);
            context.setContextPath("/");
//            String[] vitualHosts = {"10.136.130.171", "aaa"};
//            context.setVirtualHosts(vitualHosts);
            server.setHandler(context);

            context.addServlet(new ServletHolder(new SearchServlet()),
                    "/");

            //server.setHandler(new JettyReceive());

            server.start();
            server.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
