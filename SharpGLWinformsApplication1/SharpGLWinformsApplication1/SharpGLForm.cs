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
    public partial class SharpGLForm : Form
    {
        private CircularArray receivedValue;
        /// <summary>
        /// Initializes a new instance of the <see cref="SharpGLForm"/> class.
        /// </summary>
        public SharpGLForm()
        {
            InitializeComponent();
            receivedValue = new CircularArray(this.MinimumSize.Width);
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

            gl.Ortho2D(0, this.openGLControl.Size.Width, 0, this.openGLControl.Size.Height);


            gl.Color(0.0f, 1.0f, 0.0f);
            //gl.Vertex(0, 0);
            //gl.Vertex(50, 100);
            gl.Begin(OpenGL.GL_LINE_STRIP);
            for (int i = 0; i < this.openGLControl.Width - 1; i++)
            {
                if (receivedValue.getValue(i) <= 70)
                    continue;
                gl.Vertex(i,receivedValue.getValue(i));
            }
            gl.End();
            gl.Flush();

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

            this.btnRead.Enabled = true;
            this.btnStopRead.Enabled = false;
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
            string valuebuffer = this.ReadSerialData();
            Action<string> setValueAction = text => this.textBox1.Text += text;

            string[] value = valuebuffer.Split(' ');

            try
            {
                for (int i = 0; i < value.Length; i++)
                    receivedValue.addValue(Convert.ToInt32(value[i]));
            }
            catch (FormatException ex)
            {

            }

            //if (this.textBox1.InvokeRequired)
            //{
            //    for (int i = 0; i < value.Length; i++)
            //        this.textBox1.Invoke(setValueAction, value[i]+" ");
            //}
            //else
            //{
            //    setValueAction(valuebuffer);
            //}
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
                port = new SerialPort(portName, 9600);
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

    }
}
