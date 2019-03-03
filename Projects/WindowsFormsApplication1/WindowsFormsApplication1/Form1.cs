using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Management;
using System.Diagnostics;
using System.Threading;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        bool isConnected = false;
        String[] ports;
        SerialPort port;
        String command = "";

        public Form1()
        {
            InitializeComponent();
            disableControls();
            getAvailableComPorts();

            //
            //Initialize Ports
            //
            foreach (string port in ports)
            {
                comboBox1.Items.Add(port);
                Console.WriteLine(port);
                if (ports[0] != null)
                {
                    comboBox1.SelectedItem = ports[0];
                }
            }
            //
            //Select Arduino Controller as default
            //
            string autodetect = AutodetectArduinoPort();
            comboBox1.SelectedItem = autodetect;
            if (comboBox1.SelectedItem == null)
            {
                //If Arduino is not detected, set 1st item as default port
                comboBox1.SelectedItem = comboBox1.Items[0];
            }

            //
            //Initialize Lengths
            //
            comboBox2.Items.Add("25y");
            comboBox2.Items.Add("25m");
            comboBox2.SelectedIndex = 0;

            //
            //Initialize Events
            //
            comboBox3.Items.Add(25);
            comboBox3.Items.Add(50);
            comboBox3.Items.Add(75);
            comboBox3.Items.Add(100);
            comboBox3.Items.Add(200);
            comboBox3.Items.Add(400);
            comboBox3.Items.Add(500);
            comboBox3.Items.Add(800);
            comboBox3.Items.Add(1000);
            comboBox3.Items.Add(1650);
            comboBox3.SelectedIndex = 0;

        }

        void getAvailableComPorts()
        {
            ports = SerialPort.GetPortNames();
        }

        void disableControls()
        {
            comboBox2.Enabled = false;
            comboBox3.Enabled = false;
            numericUpDown1.Enabled = false;
            maskedTextBox1.Enabled = false;
            maskedTextBox2.Enabled = false;
            button2.Enabled = false;
        }

        void enableControls()
        {
            comboBox2.Enabled = true;
            comboBox3.Enabled = true;
            numericUpDown1.Enabled = true;
            maskedTextBox1.Enabled = true;
            maskedTextBox2.Enabled = true;
            button2.Enabled = true;
        }

        void sendtoArduino()
        {
            //
            //Length
            //
            if (comboBox2.SelectedIndex == 0)
            {
                command += '0';
            }
            else
            {
                command += '1';
            }
            //
            //Number of times
            //
            string val = numericUpDown1.Value.ToString();
            if (numericUpDown1.Value < 9)
            {
                command = command + '0' + val;
            }
            else
            {
                command = command + val;
            }
            
            //
            //Pace
            //
            command += maskedTextBox1.Text;
            string total_seconds = convert(maskedTextBox1.Text);
            total_seconds = format(total_seconds);
            //port.Write("#PACE" + total_seconds + "\n");
            Debug.WriteLine("Pace: " + total_seconds);
            
            //
            //Interval
            //
            command += maskedTextBox2.Text;
            string interval = convert(maskedTextBox2.Text);
            interval = format(interval);
            //port.Write("#REST" + interval + "\n");
            Debug.WriteLine("Interval: " + interval);
            
            //
            //Distance
            //
            string s_dist = comboBox3.SelectedItem.ToString();
            int dist = Int32.Parse(s_dist);
            if (dist < 100) command += "00";
            else if (dist < 1000) command += "0"; 
            command += s_dist;
            
            if (dist > 9)
            {
                
                Debug.WriteLine("Distance: " + s_dist);
            }
            else
            {
                
                Debug.WriteLine("Distance: " + s_dist);
            }
           
            command += '\n';
            port.Write(command);
            Debug.WriteLine(command);
            command = "";
        }

        private void connectToArduino()
        {
            isConnected = true;
            string selectedPort = comboBox1.GetItemText(comboBox1.SelectedItem);
            try
            {
                port = new SerialPort(selectedPort, 9600, Parity.None, 8, StopBits.One);
            }
            catch (ManagementException e)
            {
                //Do Nothing
            }
            port.Open();
            
            button1.Text = "Disconnect";
        }

        private void disconnectFromArduino()
        {
            isConnected = false;
            
            port.Close();
            button1.Text = "Connect";
        }

        private string AutodetectArduinoPort()
        {
            //Set Arduino port as default
            ManagementScope connectionScope = new ManagementScope();
            SelectQuery serialQuery = new SelectQuery("SELECT * FROM Win32_SerialPort");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(connectionScope, serialQuery);

            try
            {
                foreach (ManagementObject item in searcher.Get())
                {
                    string desc = item["Description"].ToString();
                    string deviceId = item["DeviceID"].ToString();

                    if (desc.Contains("Arduino"))
                    {
                        return deviceId;
                    }
                }
            }
            catch (ManagementException e)
            {
                //Do Nothing
            }

            return null;
        }

        string convert(string s)
        {
            /*convert string from masked textbox to number for second conversion
            and then back to string to send to arduino*/
            int minutes;
            int seconds;
            double milliseconds;
            double total_seconds;
            string s_total_seconds;
            try
            {
                minutes = Int32.Parse(s.Substring(0, 2));
                seconds = Int32.Parse(s.Substring(3, 2));
                milliseconds = Double.Parse(s.Substring(6, 2));
                total_seconds = (minutes * 60) + seconds + (milliseconds * .01);
                s_total_seconds = total_seconds.ToString();
                if (milliseconds == 0)
                {
                    s_total_seconds += ".00";
                }
                return s_total_seconds;
            }
            catch (FormatException)
            {
                //Do Nothing
            }
            return "";
        }

        string format(string s)
        {
            //Make sure all strings are the same # of characters
            for (int i = s.Length; i < 7; i++)
            {
                s += "0";
            }
            return s;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!isConnected)
            {
                connectToArduino();
                enableControls();
            }
            else
            {
                disconnectFromArduino();
                disableControls();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (maskedTextBox1.Text != "  :  :" && maskedTextBox2.Text != "  :  :" && comboBox2.SelectedItem != null && comboBox3.SelectedItem != null)
            {
                sendtoArduino();
                
            }
        }
    }
}
