package backend;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class Client implements ActionListener
{
	private JFrame _window = new JFrame( "Client Chat Window" );
	private JPanel _masterPane = new JPanel(), _buttonPane = new JPanel();
	private JLabel _prompt = new JLabel( "Enter your message in the box below");
	private JTextField _inputField = new JTextField();
	private JButton _sendButton = new JButton( "Send" ), _exitButton = new JButton( "Exit Chat" );
	
	
	private Socket _outputSocket = null, _inputSocket;
	
	private String _serverAddress = "192.168.2.8";
	private static final int PORT_NUMBER_OUTPUT = 9091, PORT_NUMBER_INPUT = 9090;
	
	private boolean _keepRunning = true;
	
	public Client() throws UnknownHostException, IOException
	{
		GridLayout grid = new GridLayout(3,1);
		
		_masterPane.setLayout(grid);
		
		_masterPane.add(_prompt);
		_masterPane.add(_inputField);
		
		_buttonPane.add(_sendButton, BorderLayout.WEST);
		_buttonPane.add(_exitButton, BorderLayout.EAST);
		
		_masterPane.add(_buttonPane);
		
		_sendButton.addActionListener(this);
		_exitButton.addActionListener(this);
		
		_window.add(_masterPane);
		
		_window.pack();
		_window.setVisible(true);
		_window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		
		
		
//		_serverAddress = JOptionPane.showInputDialog("Enter IP Address of a machine that is running the date service on port 9090:");
		
		receiveMessage();
		
	}
	
	private void receiveMessage() throws IOException
	{
		try
		{
			while( _keepRunning )
			{
//			print("waiting for input socket to open");
				_inputSocket = new Socket( _serverAddress, PORT_NUMBER_INPUT );	// slight pause (one second maybe) after this line
				
//			print("socket open, waiting for input");
				
				BufferedReader buffer = new BufferedReader( new InputStreamReader( _inputSocket.getInputStream() ) );	// waits for server to send message at this line
				
				System.out.println( "Server $\n\t" + buffer.readLine() );
				
				_inputSocket.close();
				buffer.close();
//			sendMessage( "I heard that!" );
				
				
				
			}
		}
		catch( SocketException e )
		{
			System.out.println("<<<          Connection closed.          >>>\n"
					+ 		   "<<<  Thanks for chatting with us today!  >>>");
		}
		
	}
	
	private void sendMessage( String message ) throws IOException
	{
		
//		dbug();
		
		_outputSocket = new Socket( _serverAddress, PORT_NUMBER_OUTPUT );
		
		PrintWriter printer = new PrintWriter( _outputSocket.getOutputStream() );
		printer.println( message );	// actually printing the message might help debug...
		
		printer.flush();
		printer.close();
		
		System.out.println( "\t\t\tClient $\n\t\t\t\t" + message );
		
//		print("message should have sent");
	}
	
	public void dbug()
	{
		System.out.println( "Reaching");
	}
	
	public void print(String message)
	{
		System.out.println(message);
	}

	@Override
	public void actionPerformed(ActionEvent e0)
	{
		String cmd = e0.getActionCommand();
		
		switch( cmd )
		{
			case "Send":
			{
				
				try
				{
					sendMessage( _inputField.getText() );
					_inputField.setText("");
				}
				catch(IOException e)
				{
					System.out.println( " <<< Unable to send.  Please try again! >>>");
				}
				
				break;
				
			}
			case "Exit Chat":
			{
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
				break;
			}
		}
	}
}
