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

            _excelApp = new Excel.Application();
            _excelApp.DisplayAlerts = false;

            _doneMessage = "Converting " + _sourceExtention + " to " + _destExtention + " is done";
        }

        public void MainWindow_Closed(object sender, EventArgs e)
        {
            _excelApp.Quit();
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
                    !fileName.Contains(_tempFileName));
        }

        private void AddFullPathInList(String extension, String fullPath)
        {
            if (IsRightFile(extension, fullPath))
                FullPathListBox.Items.Add(fullPath);
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
                        AddFullPathInListFromDirectory(_sourceExtention, fullPath);

                    else
                        AddFullPathInList(_sourceExtention, fullPath);
                }
            }
        }

        private void ConvertAndSaveFile(Excel.Worksheet workSheet)
        {
            String directory = Path.GetDirectoryName(_sourceFullPath);
            directory += "\\";

            String serverDirectory = directory + "Server\\";
            String clientDirectory = directory + "Client\\";

            DirectoryInfo directoryInfo = new DirectoryInfo(serverDirectory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();

            directoryInfo = new DirectoryInfo(clientDirectory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();

            if (_checkDeleteColumnValue == workSheet.Range[_checkDeleteColumnCell].Value2.ToString())
                workSheet.Columns[_deleteColumn].Delete();

            var destBook = _excelApp.Workbooks.Add(1);
            Excel.Worksheet destSheet = GetWorkSheet("Client", workSheet, destBook);
            if(null != destSheet)
                destSheet.SaveAs(serverDirectory + workSheet.Name + _destExtention, Excel.XlFileFormat.xlCSV);

            destBook.Close(false);

            destBook = _excelApp.Workbooks.Add(1);
            destSheet = GetWorkSheet("Server", workSheet, destBook);
            if (null != destSheet)
                destSheet.SaveAs(clientDirectory + workSheet.Name + _destExtention, _xlCSVUTF8);

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
                if (deleteName == name || "Reference" == name)
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

            if ("Client" == deleteName)
                ((Excel.Range)destSheet.Rows[1, Type.Missing]).Delete(Excel.XlDeleteShiftDirection.xlShiftUp);
            else
                ((Excel.Range)destSheet.Rows[2, Type.Missing]).Delete(Excel.XlDeleteShiftDirection.xlShiftUp);

            return destSheet;
        }

        private void ExcelToCSVButton_Click(object sender, RoutedEventArgs e)
        {
            if (0 == FullPathListBox.Items.Count)
            {
                MessageBox.Show(_infoMessage, "INFO");
                return;
            }

            try
            {
                foreach (var item in FullPathListBox.Items)
                {
                    _sourceFullPath = item.ToString();

                    Excel.Workbook excelWorkbook = _excelApp.Workbooks.Open(_sourceFullPath);
                    excelWorkbook = _excelApp.ActiveWorkbook;

                    foreach (Excel.Worksheet workSheet in excelWorkbook.Worksheets)
                    {
                        if (workSheet.Name.Contains(_skipSheetName))
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

            MessageBox.Show(_doneMessage, "DONE");
        }


        private void ClearFullPathListButton_Click(object sender, RoutedEventArgs e)
        {
           FullPathListBox.Items.Clear();
        }

        private Excel.Application _excelApp;

        private String _tempFileName = "~$";

        private String _sourceExtention = ".xlsx";
        private String _destExtention = ".csv";

        private String _sourceFullPath;

        private String _skipSheetName = "ForReference_";

        private String _infoMessage = "Plz drag and drop the file or the folder";
        private String _doneMessage;

        private String _deleteColumn = "A";
        private String _checkDeleteColumnCell = "A2";
        private String _checkDeleteColumnValue = "Index";
        private int _xlCSVUTF8 = 62;
    }
}
