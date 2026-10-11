#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QWidget>

class StoreImporter final : public QMainWindow {
public:
 StoreImporter() {
  setWindowTitle("Spectra Game Store Importer"); resize(760,620); setMinimumSize(640,500);
  auto *root=new QWidget; auto *layout=new QVBoxLayout(root);
  auto *title=new QLabel("<h2>SPECTRA / GAME STORE IMPORTER</h2><p>Import a Store package into a Spectra version folder.</p>");
  title->setWordWrap(true); layout->addWidget(title);
  auto *info=new QLabel("Choose the folder containing store-manifest.json, then choose the target Spectra version folder. The importer copies files only; it never executes imported files or installs retail PlayStation packages.");
  info->setWordWrap(true); layout->addWidget(info);
  auto *sourceRow=new QHBoxLayout; sourceLabel_=new QLabel("Source package: not selected");sourceLabel_->setWordWrap(true);auto *sourceButton=new QPushButton("Choose package folder");sourceRow->addWidget(sourceLabel_,1);sourceRow->addWidget(sourceButton);layout->addLayout(sourceRow);
  auto *destRow=new QHBoxLayout;destLabel_=new QLabel("Spectra version folder: not selected");destLabel_->setWordWrap(true);auto *destButton=new QPushButton("Choose version folder");destRow->addWidget(destLabel_,1);destRow->addWidget(destButton);layout->addLayout(destRow);
  auto *install=new QPushButton("IMPORT SPECTRA GAME STORE");install->setObjectName("primary");layout->addWidget(install);
  progress_=new QProgressBar;progress_->setRange(0,100);progress_->setValue(0);layout->addWidget(progress_);
  layout->addWidget(new QLabel("Import activity"));log_=new QTextEdit;log_->setReadOnly(true);layout->addWidget(log_,1);
  setStyleSheet("QMainWindow,QWidget{background:#0b1424;color:#edf4ff} QLabel{color:#edf4ff} QPushButton{background:#173a68;color:white;border:1px solid #4275b7;border-radius:8px;padding:10px} QPushButton#primary{background:#c8e2ff;color:#092044;font-weight:800} QTextEdit{background:#101f35;color:#edf4ff;border:1px solid #29496f}");
  setCentralWidget(root);
  connect(sourceButton,&QPushButton::clicked,this,[this]{QString p=QFileDialog::getExistingDirectory(this,"Select Store package folder");if(!p.isEmpty()){source_=p;sourceLabel_->setText(p);log("Source selected: "+p);}});
  connect(destButton,&QPushButton::clicked,this,[this]{QString p=QFileDialog::getExistingDirectory(this,"Select Spectra version folder",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/Spectra Library/v2.1");if(!p.isEmpty()){destination_=p;destLabel_->setText(p);log("Destination selected: "+p);}});
  connect(install,&QPushButton::clicked,this,[this]{runImport();});
 }
private:
 QLabel *sourceLabel_{},*destLabel_{};QProgressBar *progress_{};QTextEdit *log_{};QString source_,destination_;
 void log(const QString&s){log_->append("["+QDateTime::currentDateTime().toString("hh:mm:ss")+"] "+s.toHtmlEscaped());}
 void runImport(){
  if(source_.isEmpty()||destination_.isEmpty()){QMessageBox::information(this,"Choose folders","Select both a Store package folder and a Spectra version folder first.");return;}
  if(!QFileInfo(QDir(source_).filePath("store-manifest.json")).isFile()){QMessageBox::warning(this,"Invalid package","The source folder must contain store-manifest.json.");return;}
  if(QFileInfo(source_).canonicalFilePath()==QFileInfo(destination_).canonicalFilePath()){QMessageBox::warning(this,"Invalid destination","The source and destination cannot be the same folder.");return;}
  const QString target=QDir(destination_).filePath("Spectra Game Store");
  if(QFileInfo::exists(target)){QMessageBox::warning(this,"Already installed","The destination already contains a Spectra Game Store folder. Rename or move it before importing to avoid overwriting files.");return;}
  if(QMessageBox::question(this,"Confirm import","Copy the selected Store package into:\n"+target+"\n\nNo files will be executed.",QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel)!=QMessageBox::Yes)return;
  QStringList files;QDirIterator scan(source_,QDir::Files|QDir::NoDotAndDotDot,QDirIterator::Subdirectories);while(scan.hasNext())files<<scan.next();
  if(files.isEmpty()){QMessageBox::warning(this,"Empty package","The selected package contains no files.");return;}
  if(!QDir().mkpath(target)){QMessageBox::critical(this,"Import failed","Could not create destination folder.");return;}
  int copied=0,failed=0;progress_->setRange(0,files.size());
  for(int i=0;i<files.size();++i){const QString from=files[i];const QString rel=QDir(source_).relativeFilePath(from);const QString to=QDir(target).filePath(rel);QDir().mkpath(QFileInfo(to).absolutePath());if(QFile::copy(from,to)){++copied;log("Copied: "+rel);}else{++failed;log("FAILED: "+rel);}progress_->setValue(i+1);QApplication::processEvents();}
  log(QString("Finished: %1 copied, %2 failed.").arg(copied).arg(failed));
  QMessageBox::information(this,failed?"Import completed with errors":"Import complete",QString("Copied: %1\nFailed: %2\nDestination: %3").arg(copied).arg(failed).arg(target));
 }
};
int main(int argc,char**argv){const QByteArray pluginPath=QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath().toLocal8Bit()+"/platforms";qputenv("QT_QPA_PLATFORM_PLUGIN_PATH",pluginPath);QApplication app(argc,argv);StoreImporter window;window.show();return app.exec();}
