#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>
#include <string>

QSqlDatabase database;

int connectDB(void)
{
    int ret = -1;

    QFile dbFile("test.db");
    if (!dbFile.exists())
    {
        QByteArray data;
        QFile fileSrc("assets:/db/test.db");
        if (fileSrc.open(QFile::ReadOnly)) {
            data = fileSrc.readAll();
            fileSrc.close();
        } else {
            qDebug() << "Error: Src file open failed." << fileSrc.fileName() << fileSrc.errorString();
            return ret;
        }
        if (dbFile.open(QFile::WriteOnly)) {
            dbFile.write(data);
            dbFile.close();
            qDebug() << "Write ok!";
        } else {
            qDebug() << "Error: Database file open failed" << dbFile.fileName() << dbFile.errorString();
            return ret;
        }
    }
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("test.db");
    database.setUserName("sa");
    database.setPassword("123456");
    ret = database.open();

    if(ret){
        qDebug() << "Connect ok!";
    }else{
        qDebug() << "Error: Failed to connect database.";
    }

    return true;
}

int create_table()
{
    int ret = -1;
    QSqlQuery sql_query;
    QString create_sql = "create table student(id integer primary key, name text, age inegert)";
    sql_query.prepare(create_sql);
    ret = sql_query.exec();
    if(ret){
        qDebug() << "Table created!";
    }else{
        qDebug() << "Error: Fail to create table.";
    }

    return ret;
}

int insertTo(QString insert_sql)
{
    int ret = true;
    QSqlQuery sql_query;
    ret = sql_query.exec(insert_sql);
    if(ret){
        qDebug() << "Insert complete!";
    }else{
        qDebug() << "Error: Fail to insert.";
    }

    return ret;
}

int search()
{
    int ret = true;
    QSqlQuery sql_query;
    QString select_sql = "select id, name from student";
    sql_query.prepare(select_sql);
    ret = sql_query.exec();

    if(ret){
        while(sql_query.next())
        {
            int id = sql_query.value(0).toInt();
            QString name = sql_query.value(1).toString();
            qDebug() << "id:"<< id <<"\tname:" << name;
        }
    }else{
        qDebug() << "Error: Fail to serach.";
    }

    return ret;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Start connect database";
    connectDB();
    //create_table();

    search();
    QString insert_sql = "insert into student values (1, \"Tang\", 19)";
    insertTo(insert_sql);

    search();
    database.close();
    qDebug() << "End";

    return a.exec();
}
