using System;
using System.Drawing;
using System.Windows.Forms;

class MForm : Form {
    public MForm() {
        Text = "Simple menu";

        MenuStrip ms = new MenuStrip();
        ms.Parent = this;
        
        ToolStripMenuItem file = new ToolStripMenuItem("&File");          
        ToolStripMenuItem exit = new ToolStripMenuItem("&Exit", null,
            new EventHandler(OnExit));          
        exit.ShortcutKeys = Keys.Control | Keys.X;
        file.DropDownItems.Add(exit);

        ms.Items.Add(file);
        MainMenuStrip = ms;
        Size = new Size(250, 200);

        CenterToScreen();
    }

    void OnExit(object sender, EventArgs e) {
       Close();
    }
}

class MApplication {
    public static void Main() {
        Application.Run(new MForm());
    }
}