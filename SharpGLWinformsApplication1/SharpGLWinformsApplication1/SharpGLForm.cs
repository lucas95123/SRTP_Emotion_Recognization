using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using SharpGL;

namespace SharpGLWinformsApplication1
{
    /// <summary>
    /// The main form class.
    /// </summary>
    /// 
    public partial class SharpGLForm : Form
    {
        private CircularArray receivedValue;//heart beat
        private CircularArray Evalue;//Muscle
        private CircularArray Svalue;//Skin
        /// <summary>
        /// Initializes a new instance of the <see cref="SharpGLForm"/> class.
        /// </summary>
        public SharpGLForm()
        {
            InitializeComponent();
            receivedValue = new CircularArray(this.MinimumSize.Width-200);
            Svalue = new CircularArray(this.MinimumSize.Width - 200);
            Evalue = new CircularArray(this.MinimumSize.Width - 200); 
        }

        private void SharpGLForm_Load(object sender, EventArgs e)
        {
            this.toolStripComboBox1.Items.AddRange(SerialPort.GetPortNames());                                 // find the ardunio port
            this.toolStripComboBox1.SelectedIndex = this.toolStripComboBox1.Items.Count - 1;            // select the arduino port
            this.btnRead.Enabled = true;
            this.btnStopRead.Enabled = false;
            this.btnArduinoStartSend.Enabled = this.btnArduinoStopSend.Enabled = false;
        }

        /// <summary>
        /// Handles the OpenGLDraw event of the openGLControl control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="RenderEventArgs"/> instance containing the event data.</param>
        private void openGLControl_OpenGLDraw(object sender, RenderEventArgs e)
        {
            OpenGL gl = openGLControl.OpenGL;

            gl.MatrixMode(OpenGL.GL_PROJECTION);
            //  Clear the color and depth buffer.
            gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);

            //  Load the identity matrix.
            gl.LoadIdentity();

            gl.Ortho2D(0, this.openGLControl.Size.Width, 0, this.openGLControl.Size.Height * 2);

            gl.Color(1.0f,1.0f,1.0f);
            gl.Begin(OpenGL.GL_LINE_STRIP);
                gl.Vertex(1.0d,0.0d);
                gl.Vertex(1.0d, this.openGLControl.Size.Height*3);
                for (int i = 0; i < 3;i++ )
                {
                    for(int j=0;j<openGLControl.Size.Height;j+=5)
                    {
                        gl.Vertex(0.0d, i * openGLControl.Size.Height + j);
                        gl.Vertex(1.0d, i * openGLControl.Size.Height + j);
                    }
                }
                    gl.End();
            gl.Color(0.0f, 1.0f, 0.0f);
            //gl.Vertex(0, 0);
            //gl.Vertex(50, 100);
            gl.Begin(OpenGL.GL_LINE_STRIP);//|
            gl.Vertex(this.openGLControl.Width - 200, 0);
            gl.Vertex(this.openGLControl.Width - 200, this.openGLControl.Size.Height*3 - 1);
            gl.End();
            int Height = this.openGLControl.Size.Height;
            for (int i = 0; i < 24; i ++)
            {
                gl.Begin(OpenGL.GL_LINE_STRIP);
                gl.Vertex(0, i*Height/12);
                gl.Vertex(5, i*Height/12);
                gl.End();
            }
                
            /* gl.Color(1.0, 0.0f, 0.0f);
            gl.Begin(OpenGL.GL_LINE_STRIP);
            for (int i = 10; i < this.openGLControl.Width - 200; i++)
            {
                if (receivedValue.getValue(i) < 60)
                    continue;
                gl.Vertex(i, receivedValue.getValue(i));
            }
            gl.End();*/
            gl.Color(0.0f, 1.0f, 0.0f);
            gl.Begin(OpenGL.GL_LINE_STRIP);
            gl.Vertex(1, this.openGLControl.Size.Height);
            gl.Vertex(openGLControl.Size.Width - 200, this.openGLControl.Size.Height);
            gl.End();
            gl.Color(1.0f, 0.0f, 0.0f);
            gl.Begin(OpenGL.GL_LINE_STRIP);
            for (int i = 10; i < this.openGLControl.Width - 200; i++)
            {
                if (Svalue.getValue(i) < 60||Svalue.getValue(i)>650)
                    continue;
                gl.Vertex(i, Svalue.getValue(i)+0);
            }
            gl.End();
            gl.Color(1.0f, 1.0f, 0.0f);
            gl.Begin(OpenGL.GL_LINE_STRIP);
            for (int i = 10; i < this.openGLControl.Width - 200; i++)
            {
                if (Evalue.getValue(i) < 60||Evalue.getValue(i)>200)
                    continue;
                gl.Vertex(i, Evalue.getValue(i) + this.openGLControl.Size.Height);
            }
            gl.End();
            //for (int i=10;)
            gl.Flush();
            label1.Text="GSR   "+Svalue.newest.ToString();
            label2.Text="EMG   "+Evalue.newest.ToString();
            label3.Text="HB   "+receivedValue.newest.ToString();
        }

        /// <summary>
        /// Handles the OpenGLInitialized event of the openGLControl control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void openGLControl_OpenGLInitialized(object sender, EventArgs e)
        {
            //  TODO: Initialise OpenGL here.

            //  Get the OpenGL object.
            OpenGL gl = openGLControl.OpenGL;

            //  Set the clear color.
            gl.ClearColor(0, 0, 0, 0);
        }

        /// <summary>
        /// Handles the Resized event of the openGLControl control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void openGLControl_Resized(object sender, EventArgs e)
        {
            //  TODO: Set the projection matrix here.

            //  Get the OpenGL object.
            OpenGL gl = openGLControl.OpenGL;

            //  Set the projection matrix.
            gl.MatrixMode(OpenGL.GL_PROJECTION);

            //  Load the identity.
            gl.LoadIdentity();

            //  Create a perspective transformation.
            gl.Perspective(60.0f, (double)Width / (double)Height, 0.01, 100.0);

            //  Use the 'look at' helper function to position and aim the camera.
            gl.LookAt(-5, 5, -5, 0, 0, 0, 0, 1, 0);

            //  Set the modelview matrix.
            gl.MatrixMode(OpenGL.GL_MODELVIEW);
        }

        /// <summary>
        /// The current rotation.
        /// </summary>

        private void btnRead_Click(object sender, EventArgs e)
        {
            this.InitializeSerialPort();

            this.btnRead.Enabled = false;
            this.btnStopRead.Enabled = true;
            this.btnArduinoStartSend.Enabled = false;
            this.btnArduinoStopSend.Enabled = true;
        }


        private void btnStopRead_Click(object sender, EventArgs e)
        {
            this.DisposeSerialPort();

            this.btnStopRead.Enabled = false;
            this.btnArduinoStartSend.Enabled = this.btnArduinoStopSend.Enabled = false;
            this.btnRead.Enabled = true;
        }

        void RefreshInfoTextBox()
        {
            string value = this.ReadSerialData();
            Action<string> setValueAction = text => this.textBox1.Text += text;
            string[] valueBuffer = value.Split(new string[] { "\r\n" }, StringSplitOptions.None);
            for (int i = 0; i < valueBuffer.Length; i++)
            {
                try
                {
                    switch (valueBuffer[i][0]) { 
                        case 'B':receivedValue.addValue(Convert.ToInt32(valueBuffer[i].Substring(1)));
                            break;
                        case 'S': Svalue.addValue(Convert.ToInt32(valueBuffer[i].Substring(1)));
                            break;
                        case 'E': Evalue.addValue(Convert.ToInt32(valueBuffer[i].Substring(1)));break;
                    }
                }
                catch (Exception ex)
                {

                }
            }

            if (this.textBox1.InvokeRequired)
            {
                this.textBox1.Invoke(setValueAction, value);
            }
            else
            {
                setValueAction(value);
            }
        }

        void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            this.RefreshInfoTextBox();
        }

        private SerialPort port = null;

        private void ChangeArduinoSendStatus(bool allowSend)
        {
            if (port != null && port.IsOpen)
            {
                if (allowSend)
                {
                    port.WriteLine("serial start");
                }
                else
                {
                    port.WriteLine("serial stop");
                }
            }
        }
        private void InitializeSerialPort()
        {
            try
            {
                string portName = this.toolStripComboBox1.SelectedItem.ToString();
                port = new SerialPort(portName, 115200);
                port.Encoding = Encoding.ASCII;
                port.DataReceived += port_DataReceived;
                port.Open();
                this.ChangeArduinoSendStatus(true);
            }
            catch (Exception ex)
            {
                MessageBox.Show("关闭串口发生错误" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void DisposeSerialPort()
        {
            if (port != null)
            {
                try
                {
                    this.ChangeArduinoSendStatus(false);
                    if (port.IsOpen)
                    {
                        port.Close();
                    }
                    port.Dispose();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("关闭串口发生错误：" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private string ReadSerialData()
        {
            string value = "";
            try
            {
                if (port != null && port.BytesToRead > 0)
                {
                    value = port.ReadExisting();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("读取串口数据发生错误：" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return value;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            textBox1.SelectionStart = textBox1.Text.Length;
            textBox1.ScrollToCaret();
        }

        private void btnArduinoStartSend_Click(object sender, EventArgs e)
        {
            this.btnArduinoStartSend.Enabled = false;
            this.btnArduinoStopSend.Enabled = true;
            this.ChangeArduinoSendStatus(true);
        }

        private void btnArduinoStopSend_Click(object sender, EventArgs e)
        {
            this.btnArduinoStartSend.Enabled = true;
            this.btnArduinoStopSend.Enabled = false;
            this.ChangeArduinoSendStatus(false);
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            this.textBox1.Text = "";
            receivedValue.empty();
        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

    }
}