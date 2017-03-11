package backend;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

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
						
					}
					catch(Exception e)
					{
						e.printStackTrace();
						System.out.println( "Doo-wah-ditty-ditty-dum-ditty-doo" );
					}
						socket.close();
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
