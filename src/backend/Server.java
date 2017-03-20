package backend;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class Server implements ActionListener
{
	
	private JFrame _window = new JFrame("Chat");
	private JPanel _masterPane = new JPanel(), _subPane = new JPanel();
	private JLabel _prompt = new JLabel( "Enter your message in the box below");
	private JTextField _inputField = new JTextField();
	private JButton _sendButton = new JButton( "Send" ), _exitButton = new JButton("Exit Chat");
	
	private boolean _keepRunning = true;
	
	private static final int PORT_NUMBER_OUTPUT = 9090;
	private static final int PORT_NUMBER_INPUT = 9091;
	
	private ServerSocket _outputServerSocket = null, _inputServerSocket = null;
	private Socket _outputSocket = null, _inputSocket = null;
	
	
	
	public Server()
	{
		
		GridLayout grid = new GridLayout(3,1);
		
		_masterPane.setLayout(grid);
		
		_masterPane.add(_prompt);
		_masterPane.add(_inputField);
//		_masterPane.add(_sendButton);
		
		_subPane.add(_sendButton, BorderLayout.WEST);
		_subPane.add(_exitButton, BorderLayout.EAST);
		
		_masterPane.add(_subPane);
		
		_sendButton.addActionListener(this);
		_exitButton.addActionListener(this);
		
		_window.add(_masterPane);
		
		_window.pack();
		
		_window.setVisible(true);
		
		_window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		openChat();
		
	}
	
	private void openChat()
	{
		if( !_keepRunning)
		{
			return;
		}
		
		try
		{
//			print("about to wait for input connection.");
			_inputServerSocket = new ServerSocket( PORT_NUMBER_INPUT );
//			print("input connection received.  About to wait for output connection.");
			_outputServerSocket = new ServerSocket( PORT_NUMBER_OUTPUT );
//			print("output connection received.");
			
			receiveMessage();	// actually receiving the message also might help debugging...
			
			
		}
		catch( IOException e )
		{
			System.out.println( "\t<<< Unable to initialize chat.  Please try again! >>>" );
			e.printStackTrace();
		}
	}
	
	private void receiveMessage() throws IOException
	{
		try
		{
			while( _keepRunning )
			{
//			dbug();
//			print( "Waiting for input from client");
				_inputSocket = _inputServerSocket.accept();
				
//			print( "Input received from client");
				
				BufferedReader buffer = new BufferedReader( new InputStreamReader( _inputSocket.getInputStream()) );
				
				System.out.println( "Client $\n\t" + buffer.readLine() );
				
				buffer.close();
				_inputSocket.close();
				
				
			}
		}
		catch(SocketException e)
		{
			System.out.println("<<<            Connection closed.           >>>\n"
					+          "<<<  Thank you for chatting with us today!  >>>");
		}
		
	}
	
	private void sendMessage(String message) throws IOException
	{
//		dbug();
		
		_outputSocket = _outputServerSocket.accept();
		
		PrintWriter printer = new PrintWriter( _outputSocket.getOutputStream() );
		printer.println(message);
		
		printer.flush();
		printer.close();
		
		_outputSocket.close();
		
		System.out.println("\t\t\t\tServer $\n\t\t\t" + message);
		
	}

	@Override
	public void actionPerformed(ActionEvent e0)
	{
		String cmd = e0.getActionCommand();
		
//		print( "action command:  \"" + cmd + "\"");
		
		switch( cmd )
		{
			case "Send":
			{
//				_message = _inputField.getText();
				String message = "";
//				String printEcho = "\t\t\tServer $\n\t\t\t\t";
				try
				{
					message = _inputField.getText();
					
//					print("((( should print \"" + message + "\" )))");
					
					sendMessage( message );

//					sendMessage( (message = _inputField.getText()) );
					_inputField.setText("");
//					printEcho += message;
				}
				catch(IOException e1)
				{
					System.out.println( " <<< Unable to send.  Please try again! >>>" );
				}
				
//				System.out.println( printEcho );
				
				break;
			}
			case "Exit Chat":
			{
				try
				{
					_inputServerSocket.close();
				}
				catch( Exception e )
				{
				}
				
				try
				{
					_outputServerSocket.close();
				}
				catch( Exception e)
				{
				}
				
				try
				{
					_inputSocket.close();
				}
				catch( Exception e )
				{
				}
				
				try
				{
					_outputSocket.close();
				}
				catch( Exception e )
				{
				}
				
				_window.dispose();
				
				
				
				_keepRunning = false;
				
//				print( "should close" );
				break;
			}
		}
		
	}
	
	private void dbug()
	{
		System.out.println("Reaching");
	}
	
	private void print(String msg)
	{
		System.out.println(msg);
	}
}
