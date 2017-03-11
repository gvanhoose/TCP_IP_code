package backend;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.UnknownHostException;

import javax.swing.JOptionPane;

public class Client
{
	public Client() throws UnknownHostException, IOException
	{
		
		String serverAddress = JOptionPane.showInputDialog("Enter IP Address of a machine that is running the date service on port 9090:");
		
		Socket s = new Socket( serverAddress, 9090 );
		
		BufferedReader input = new BufferedReader( new InputStreamReader( s.getInputStream() ) );
		
		String answer = "";
		
		while( "q".compareToIgnoreCase(answer) != 0 )
		{
			answer = input.readLine();
						
			JOptionPane.showMessageDialog( null, answer );
		}
		
		
		
		s.close();	// added by me, because nobody likes a resource leak!
		
		System.exit(0);
//        String serverAddress = JOptionPane.showInputDialog(
//                "Enter IP Address of a machine that is\n" +
//                "running the date service on port 9090:");
//            Socket s = new Socket(serverAddress, 9090);
//            BufferedReader input =
//                new BufferedReader(new InputStreamReader(s.getInputStream()));
//            String answer = input.readLine();
//            JOptionPane.showMessageDialog(null, answer);
//            System.exit(0);
	}
	
	public void print(String message)
	{
		System.out.println(message);
	}
}
