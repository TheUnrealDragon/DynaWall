#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::ifstream f(config);
    this->resize(1280,800);
    labelgrid = new QWidget(this);
    labelgridlayout = new QGridLayout(this);
    labelgrid->setLayout(labelgridlayout);
    ui->scrollArea->setWidget(labelgrid);
    if(f.is_open())
    {
        std::cout << "file read start" << std::endl;
        std::string str;
        std::stringstream buffer;
        buffer << f.rdbuf();
        str = buffer.str();
        std::cout << "file read complete" << std::endl;
        QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(str));
        std::cout << "==============" << std::endl;
        std::cout << str << std::endl;
        std::cout << "==============" << std::endl;
        topsettings = doc.object();
        QJsonObject settings = topsettings["ApplicationSettings"].toObject();

        //Config 1: Folder Path
        if(settings["wallpaperpath"].isNull() || settings["wallpaperpath"].isUndefined())
        {
            std::cout << "[WARN] No Wallpaper Directory Selected (Ignore if running for first time)" << std::endl;
        }
        else
        {
            std::cout << "[INFO] Directory: " << settings["wallpaperpath"].toString().toStdString() << std::endl;
            ui->wallpaperFolder->setText(settings["wallpaperpath"].toString());
            on_pushButton_clicked();
        }
    }
    else
    {
        std::cout << "Creating Config File in Application Working Directory" << std::endl;
        QJsonObject settings;
        QJsonValue val = settings;
        QJsonObject obj{
            {"ApplicationSettings",val}
        };
        QJsonDocument doc(obj);
        QByteArray arr =  doc.toJson();
        f.close();
        std::fstream d;
        d.open(config,std::fstream::out);
        if(!d.is_open())
        {
            std::cerr << "Cannot write to config file!" << std::endl;
            return;
        }
        else
        {
            d << arr.toStdString();
            d.flush();
        }
        d.close();
    }
    if(f.is_open())
        f.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->wallpaperFolder->text().isEmpty())
        return;
    std::fstream f(config);
    f.open(config,std::fstream::in);
    if(f.is_open())
    {
        //Check if the folder is the same:
        //No need to read from f as topsettings should have the read values from MainWindow Constructor
        QJsonObject settings = topsettings["ApplicationSettings"].toObject();
        if(settings.isEmpty())
        {
            std::cerr << "[WARN] Config file is corrupt: Settings empty " << std::endl;
            settings.insert("wallpaperpath",QJsonValue(ui->wallpaperFolder->text()));
            topsettings["ApplicationSettings"] = settings;
            //Write Settings and call function again!
            QJsonDocument doc(topsettings);
            f.close();
            std::fstream d;
            d.open(config,std::fstream::out);
            if(!d.is_open())
            {
                std::cerr << "[ERR] Cannot open Config file to Write" << std::endl;
                return;
            }
            d << doc.toJson().toStdString();
            d.flush();
            d.close();
            on_pushButton_clicked();
            return;
        }
        QJsonValue path = settings["wallpaperpath"];
        if(path.isUndefined())
        {
            std::cerr << "[ERR] Config file is corrupt: Path Undefined" << std::endl;
            return;
        }
        else if(path.toString() == ui->wallpaperFolder->text())
        {
            //PATH IS SAME NOTHING TO DO
        }
        else
        {
            //Update path
            path = ui->wallpaperFolder->text();
            settings["wallpaperpath"] = path;
            //Write settings;
            topsettings["ApplicationSettings"] = settings;
            f.close();
            QJsonDocument writer;
            writer.setObject(topsettings);
            std::string d = writer.toJson().toStdString();
            std::fstream w;
            w.open(config,std::fstream::out);
            if(!w.is_open())
            {
                std::cerr << "[WARN] Can not write to Config file" << std::endl;
            }
            w << d;
            w.flush();
            w.close();
        }

    }
    else
    {
        std::cout << "Creating Config File in Application Working Directory" << std::endl;
        QJsonObject settings;
        QJsonValue v = ui->wallpaperFolder->text();
        settings.insert("wallpaperpath",v);
        QJsonObject obj{
            {"ApplicationSettings",settings}
        };
        QJsonDocument doc(obj);
        QByteArray arr =  doc.toJson();
        f.close();
        std::fstream d;
        d.open(config,std::fstream::out);
        if(!d.is_open())
        {
            std::cerr << "Cannot write to config file!" << std::endl;
            return;
        }
        else
        {
            d << arr.toStdString();
            d.flush();
        }
        d.close();
    }
    if(f.is_open())
        f.close();

    int i = 0;
    int j = 0;
    for(auto const &dir_entry : std::filesystem::recursive_directory_iterator{ui->wallpaperFolder->text().toStdString()})
    {
        std::cout << dir_entry << std::endl;
        std::filesystem::path path = dir_entry.path();
        std::stringstream is;
        is << dir_entry;
        std::string p;
        is >> p;
        std::cout << "p is: " << p << std::endl;
        p.erase(0,1);
        p.erase(p.size()-1);
        QPixmap pixmap(QString::fromStdString(p));
        if(pixmap.isNull())
        {
            if(path.extension() == ".heic")
            {
                std::cout << "[INFO] heic image found!" << std::endl;
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(2000ms);
            }
            std::cout << path << " is not an image!" << std::endl;
            continue;
        }
        PreviewWidget *wid = new PreviewWidget(QString::fromStdString(p),QString::fromStdString(path.filename().string()),this);
        connect(wid,&PreviewWidget::CheckBoxChecked,this,&MainWindow::on_previewWidget_checked);
        labelgridlayout->addWidget(wid,i,j++);
        if((j+1)%4 == 0)
        {
            i++;
            j=0;
        }
    }



}

void MainWindow::on_previewWidget_checked(bool check)
{
    if(check)
        std::cout << "check" << std::endl;
    else
        std::cout << "uncheck" << std::endl;
}

