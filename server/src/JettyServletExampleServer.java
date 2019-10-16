import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

public class JettyServletExampleServer {

    public static void main(String[] args) {
        try {
            Server server = new Server(11111);

            ServletContextHandler context = new ServletContextHandler(
                    ServletContextHandler.SESSIONS);
            context.setContextPath("/");
            server.setHandler(context);

            context.addServlet(new ServletHolder(new SearchServlet()),
                    "/search");

            //server.setHandler(new JettyReceive());

            server.start();
            server.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}
