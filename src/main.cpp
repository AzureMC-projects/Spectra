#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QStandardPaths>
#include <QWidget>
#include <QUrl>

class SpectraWindow final : public QMainWindow {
public:
 SpectraWindow() {
  setWindowTitle("Spectra | Inject Files"); resize(1000,720);
  imports_=QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).filePath("Spectra Imports");
  library_=QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).filePath("Spectra Library");
  QDir().mkpath(imports_); QDir().mkpath(library_); dest_=library_;
  auto *root=new QWidget; auto *v=new QVBoxLayout(root);
  auto *intro=new QLabel("<h2>SPECTRA / FILE INJECTOR</h2><p>Put files in <b>Documents/Spectra Imports</b>. Files are discovered automatically.</p><p><b>Inject</b> copies checked files into your local Spectra Library. This is ordinary file copying, not console jailbreaking or code injection.</p>"); intro->setWordWrap(true); v->addWidget(intro);
  auto *row=new QHBoxLayout; auto *open=new QPushButton("Open imports folder"); auto *scan=new QPushButton("Rescan"); auto *all=new QPushButton("Select all"); auto *none=new QPushButton("Select none");
  for(auto *b:{open,scan,all,none})row->addWidget(b); v->addLayout(row);
  v->addWidget(new QLabel("DISCOVERED FILES — tick files to inject")); files_=new QListWidget; files_->setSelectionMode(QAbstractItemView::NoSelection); v->addWidget(files_,2);
  auto *destrow=new QHBoxLayout; destLabel_=new QLabel(dest_); destLabel_->setWordWrap(true); auto *change=new QPushButton("Change destination"); destrow->addWidget(new QLabel("Destination:"));destrow->addWidget(destLabel_,1);destrow->addWidget(change);v->addLayout(destrow);
  auto *inject=new QPushButton("INJECT SELECTED FILES");inject->setObjectName("primary");v->addWidget(inject);
  progress_=new QProgressBar;v->addWidget(progress_);v->addWidget(new QLabel("Activity log"));log_=new QTextEdit;log_->setReadOnly(true);v->addWidget(log_,1);
  setStyleSheet("QMainWindow,QWidget{background:#0b1424;color:#edf4ff} QLabel{color:#edf4ff} QPushButton{background:#173a68;color:white;border:1px solid #4275b7;border-radius:8px;padding:10px} QPushButton#primary{background:#c8e2ff;color:#092044;font-weight:800} QListWidget,QTextEdit{background:#101f35;color:#edf4ff;border:1px solid #29496f}");
  setCentralWidget(root);
  connect(open,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(imports_));});
  connect(scan,&QPushButton::clicked,this,[this]{scanFiles();});
  connect(all,&QPushButton::clicked,this,[this]{checkAll(true);});connect(none,&QPushButton::clicked,this,[this]{checkAll(false);});
  connect(change,&QPushButton::clicked,this,[this]{QString p=QFileDialog::getExistingDirectory(this,"Choose destination",dest_);if(!p.isEmpty()){dest_=p;destLabel_->setText(p);}});
  connect(inject,&QPushButton::clicked,this,[this]{injectFiles();});scanFiles();
 }
private:
 QListWidget *files_{}; QTextEdit *log_{}; QLabel *destLabel_{}; QProgressBar *progress_{}; QString imports_,library_,dest_;
 void event(QString s){log_->append("["+QDateTime::currentDateTime().toString("hh:mm:ss")+"] "+s.toHtmlEscaped());}
 void scanFiles(){files_->clear();QDir d(imports_);auto list=d.entryInfoList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name);for(auto f:list){auto *i=new QListWidgetItem(f.fileName()+" ("+QString::number(f.size())+" bytes)",files_);i->setData(Qt::UserRole,f.absoluteFilePath());i->setFlags(i->flags()|Qt::ItemIsUserCheckable);i->setCheckState(Qt::Checked);}event("Found "+QString::number(list.size())+" files in "+imports_);}
 void checkAll(bool b){for(int i=0;i<files_->count();++i)files_->item(i)->setCheckState(b?Qt::Checked:Qt::Unchecked);}
 void injectFiles(){QStringList paths;for(int i=0;i<files_->count();++i)if(files_->item(i)->checkState()==Qt::Checked)paths<<files_->item(i)->data(Qt::UserRole).toString();if(paths.isEmpty()){QMessageBox::information(this,"No files","Add files to Documents/Spectra Imports and press Rescan.");return;}if(!QFileInfo(dest_).isDir()||!QFileInfo(dest_).isWritable()){QMessageBox::warning(this,"Invalid destination","Choose an existing writable folder.");return;}if(QMessageBox::question(this,"Confirm Inject","Copy "+QString::number(paths.size())+" file(s) into:\n"+dest_+"\nExisting files will be skipped.",QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel)!=QMessageBox::Yes)return;int ok=0,skip=0,fail=0;progress_->setRange(0,paths.size());for(int n=0;n<paths.size();++n){QFileInfo f(paths[n]);QString target=QDir(dest_).filePath(f.fileName());if(!f.isFile()||!f.isReadable()||QFileInfo::exists(target)){++skip;event("Skipped "+f.fileName());}else if(QFile::copy(paths[n],target)){++ok;event("Injected (copied): "+f.fileName());}else{++fail;event("Failed: "+f.fileName());}progress_->setValue(n+1);QApplication::processEvents();}event(QString("Done: copied %1, skipped %2, failed %3").arg(ok).arg(skip).arg(fail));QMessageBox::information(this,"Inject complete",QString("Copied: %1\nSkipped: %2\nFailed: %3\n\n%4").arg(ok).arg(skip).arg(fail).arg(dest_));}
};
int main(int argc,char **argv){QApplication a(argc,argv);SpectraWindow w;w.show();return a.exec();}
