#ifndef MOUSAVIEXECUTESQLFILE_H
#define MOUSAVIEXECUTESQLFILE_H

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>


class MousaviExecuteSqlFile {
    public:
        /**
         * @brief executeQueryFile
         * Read a query file and removes some unnecessary characters/strings such as comments,
         * and executes queries.
         * If there is possible to use Transaction, so this method will uses it.
         *
         * Credit to seyedmmousavi - https://gist.github.com/seyedmmousavi/ba5bdd77aca63a7825bb7091bc016b62
         * License: MIT
         */
        static void executeQueryFile(QFile &qf, QSqlDatabase &db){
            //Read query file content
            qf.open(QIODevice::ReadOnly);
            QString queryStr(qf.readAll());
            qf.close();

            //Check if SQL Driver supports Transactions
            if(db.driver()->hasFeature(QSqlDriver::Transactions)) {
                //Replace comments and tabs and new lines with space
                queryStr = queryStr.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), " ");
                //Remove waste spaces
                queryStr = queryStr.trimmed();

                //Extracting queries
                QStringList qList = queryStr.split(';', QString::SkipEmptyParts);

                //Initialize regular expression for detecting special queries (`begin transaction` and `commit`).
                //NOTE: I used new regular expression for Qt5 as recommended by Qt documentation.
                QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
                QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

                //Check if query file is already wrapped with a transaction
                bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
                if(!isStartedWithTransaction)
                  db.transaction();     //<=== not wrapped with a transaction, so we wrap it with a transaction.

                //Execute each individual queries
                foreach(const QString &s, qList) {
                  if(re_transaction.match(s).hasMatch())    //<== detecting special query
                      db.transaction();
                  else if(re_commit.match(s).hasMatch())    //<== detecting special query
                      db.commit();
                  else {
                      db.exec(s);                        //<== execute normal query
                      if(db.lastError().type() != QSqlError::NoError) {
                          qDebug() << db.lastError().text();
                          db.rollback();                    //<== rollback the transaction if there is any problem
                      }
                  }
                }
                if(!isStartedWithTransaction)
                  db.commit();          //<== ... completing of wrapping with transaction

            //Sql Driver doesn't supports transaction
            } else {

                //...so we need to remove special queries (`begin transaction` and `commit`)
                queryStr = queryStr.replace(QRegularExpression("(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)", QRegularExpression::CaseInsensitiveOption|QRegularExpression::MultilineOption), " ");
                queryStr = queryStr.trimmed();

                //Execute each individual queries
                QStringList qList = queryStr.split(';', QString::SkipEmptyParts);
                foreach(const QString &s, qList) {
                  db.exec(s);
                  if(db.lastError().type() != QSqlError::NoError) qDebug() << db.lastError().text();
                }
            }
        }
    };

#endif // MOUSAVIEXECUTESQLFILE_H
