import java.net.*;
import java.io.*;

class Client
 {
    public static void main(String[] args) throws Exception
    {
        System.out.println("Client application is running...");
        
        Socket sobj = new Socket("localhost", 2100);
        System.out.println("Client is connected to the server...");

        PrintStream ps = new PrintStream(sobj.getOutputStream());
        BufferedReader br1 = new BufferedReader(new InputStreamReader(sobj.getInputStream()));
        BufferedReader br2 = new BufferedReader(new InputStreamReader(System.in));
        String str1, str2;

        System.out.println("Messenger Application started...");

        while(true)
        {
            System.out.println("Enter message for server (type 'exit' to quit): ");
            str1 = br2.readLine();
            ps.println(str1);
            if (str1.equalsIgnoreCase("exit")) 
            {
                break;
            }
            str2 = br1.readLine();
            System.out.println("Server says : " + str2);
        }

        System.out.println("Thank you for using Messenger Application...");
        sobj.close();
    }
}
