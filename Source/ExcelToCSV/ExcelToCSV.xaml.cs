using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using Excel = Microsoft.Office.Interop.Excel;

namespace ExcelToCSV
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            excelApp = new Excel.Application();
            excelApp.DisplayAlerts = false;

            this.doneMessage = "Converting " + this.sourceExtention + " to " + this.destExtention + " is done";
        }

        public void MainWindow_Closed(object sender, EventArgs e)
        {
            excelApp.Quit();
        }

        private void FullPathListBox_DragEnterFullPathListBox(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effects = DragDropEffects.Copy | DragDropEffects.Scroll;
        }

        private bool IsDirectory(String fullPath)
        {
            FileAttributes fileAttributes = File.GetAttributes(fullPath);

            return (FileAttributes.Directory == (fileAttributes & FileAttributes.Directory));
        }

        private bool IsRightFile(String extension, String fullPath)
        {
            String fileName = Path.GetFileName(fullPath);

            return (extension == Path.GetExtension(fileName) &&
                    !fileName.Contains(this.tempFileName));
        }

        private void AddFullPathInList(String extension, String fullPath)
        {
            if (IsRightFile(extension, fullPath))
                this.FullPathListBox.Items.Add(fullPath);
        }

        private void AddFullPathInListFromDirectory(String extention, String fullPath)
        {
            DirectoryInfo directoryInfo = new DirectoryInfo(fullPath);

            foreach (FileInfo fileInfo in directoryInfo.GetFiles())
                AddFullPathInList(extention, fileInfo.FullName);
        }

        private void FullPathListBox_DragDropFullPathListBox(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] fullPathes = (string[])e.Data.GetData(DataFormats.FileDrop);

                foreach (string fullPath in fullPathes)
                {
                    if (IsDirectory(fullPath))
                        AddFullPathInListFromDirectory(this.sourceExtention, fullPath);

                    else
                        AddFullPathInList(this.sourceExtention, fullPath);
                }
            }
        }

        private void ConvertAndSaveFile(Excel.Worksheet workSheet)
        {
            String directory = Path.GetDirectoryName(this.sourceFullPath);
            directory += "\\";

            String serverDirectory = directory + "Server\\";
            String clientDirectory = directory + "Client\\";

            DirectoryInfo directoryInfo = new DirectoryInfo(serverDirectory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();

            directoryInfo = new DirectoryInfo(clientDirectory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();

            if (checkDeleteColumnValue == workSheet.Range[checkDeleteColumnCell].Value2.ToString())
            {
                workSheet.Columns[deleteColumn].Delete();
            }

            var destBook = excelApp.Workbooks.Add(1);
            Excel.Worksheet destSheet = GetWorkSheet("Client", workSheet, destBook);
            if(destSheet != null)
                destSheet.SaveAs(serverDirectory + workSheet.Name + this.destExtention, xlCSVUTF8);

            destBook.Close(false);

            destBook = excelApp.Workbooks.Add(1);
            destSheet = GetWorkSheet("Server", workSheet, destBook);
            if (destSheet != null)
                destSheet.SaveAs(clientDirectory + workSheet.Name + this.destExtention, xlCSVUTF8);

            destBook.Close(false);
        }

        public Excel.Worksheet GetWorkSheet(string deleteName, Excel.Worksheet sourceSheet, Excel.Workbook destBook)
        {
            Excel.Range usedRange = sourceSheet.UsedRange;

            List<int> deleteColumnList = new List<int>();

            int originalColumnCount = usedRange.Rows[1].Columns.Count;
            for (int i = 0; i < originalColumnCount; ++i)
            {
                string name = Convert.ToString(usedRange.Rows[1].Cells[1, i + 1].Value2);
                if (name == deleteName)
                    deleteColumnList.Add(i + 1);
            }

            if (originalColumnCount == deleteColumnList.Count)
                return null;

            sourceSheet.Copy(destBook.Sheets[1]);
            var destSheet = destBook.Worksheets[1];

            deleteColumnList.Reverse();
            foreach (int deleteColumn in deleteColumnList)
            {
                destSheet.Columns[deleteColumn].Delete(Excel.XlDeleteShiftDirection.xlShiftToLeft);
            }

            ((Excel.Range)destSheet.Rows[1, Type.Missing]).Delete(Excel.XlDeleteShiftDirection.xlShiftUp);

            if (deleteName == "Client")
                ((Excel.Range)destSheet.Rows[1, Type.Missing]).Delete(Excel.XlDeleteShiftDirection.xlShiftUp);
            else
                ((Excel.Range)destSheet.Rows[2, Type.Missing]).Delete(Excel.XlDeleteShiftDirection.xlShiftUp);

            return destSheet;
        }

        private void ExcelToCSVButton_Click(object sender, RoutedEventArgs e)
        {
            if (0 == this.FullPathListBox.Items.Count)
            {
                MessageBox.Show(infoMessage, "INFO");
                return;
            }

            try
            {
                foreach (var item in this.FullPathListBox.Items)
                {
                    this.sourceFullPath = item.ToString();

                    Excel.Workbook excelWorkbook = this.excelApp.Workbooks.Open(this.sourceFullPath);
                    excelWorkbook = this.excelApp.ActiveWorkbook;

                    foreach (Excel.Worksheet workSheet in excelWorkbook.Worksheets)
                    {
                        if (workSheet.Name.Contains(skipSheetName))
                            continue;

                        ConvertAndSaveFile(workSheet);
                    }

                    excelWorkbook.Close();
                }
            }

            catch (Exception exception)
            {
                MessageBox.Show(exception.Message, "ERROR");
                return;
            }

            MessageBox.Show(doneMessage, "DONE");
        }


        private void ClearFullPathListButton_Click(object sender, RoutedEventArgs e)
        {
            this.FullPathListBox.Items.Clear();
        }

        private Excel.Application excelApp;

        private String tempFileName = "~$";

        private String sourceExtention = ".xlsx";
        private String destExtention = ".csv";

        private String sourceFullPath;

        private String skipSheetName = "ForReference_";

        private String infoMessage = "Plz drag and drop the file or the folder";
        private String doneMessage;

        private String deleteColumn = "A";
        private String checkDeleteColumnCell = "A2";
        private String checkDeleteColumnValue = "Index";
        private int xlCSVUTF8 = 62;
    }
}
