package backend;

import java.io.IOException;
import java.net.ConnectException;
import java.net.UnknownHostException;

public class ClientMain
{
	public static void main(String[] args)
	{
		try
		{
			Client client = new Client();
		}
		catch(UnknownHostException e)
		{
			e.printStackTrace();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
	
}
