package backend;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

import javax.swing.JOptionPane;

public class Server
{
	public Server()
	{
		try
		{
			ServerSocket listener = new ServerSocket( 9090 );
			
			try
			{
				
				
				while( true )
				{
					Socket socket = listener.accept();
					
					
					
					try
					{
						PrintWriter out = new PrintWriter( socket.getOutputStream(), true );
						out.println( new Date().toString() );
						
						String greeting = JOptionPane.showInputDialog("Would you like to say anything to the client?");
						
						out.println( greeting );
						
//						out.println("q");
						
					}
					catch(Exception e)
					{
						e.printStackTrace();
						System.out.println( "Doo-wah-ditty-ditty-dum-ditty-doo" );
					}
//					keepRunning = (JOptionPane.showInputDialog("Do you wish to send another message?").compareToIgnoreCase("q") != 0);
					
					
					
					if( JOptionPane.showConfirmDialog(null, "Do you wish to continue?") != 0 )
					{
						System.out.println("You chose to close");
						socket.close();
						break;
					}
				}
			}
			catch( Exception e )
			{
				e.printStackTrace();
				System.out.println( "Is this the lazy man's way of exiting a loop?" );
			}
			listener.close();
		}
		catch( IOException e )
		{
			System.out.println( "Error on creating ServerSocket( 9090 ) object.");
		}
//	       ServerSocket listener = new ServerSocket(9090);
//	        try
//	        {
//	            while (true)
//				{
//	                Socket socket = listener.accept();
//	                try
//					{
//	                    PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
//	                    out.println(new Date().toString());
//	                }
//					finally
//					{
//	                    socket.close();
//	                }
//	            }
//	        }
//	        finally
//			{
//	            listener.close();
//	        }

	}
}
