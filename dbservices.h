﻿#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "hqdatadefines.h"
#include <QMap>
#include <QTimer>


#define DATA_SERVICE HqInfoService::instance()

class HqInfoService : public QObject
{
    Q_OBJECT
protected:
    HqInfoService(QObject *parent = 0);
    ~HqInfoService();

public:
    friend class CGarbo;
    static HqInfoService* instance();
    QDate  getLastUpdateDateOfHSGT();
    QDate  getLastUpdateDateOfShareHistory(const QString& code);

signals:
    void signalRecvRealBlockInfo(const QList<BlockRealInfo>& list);
    void signalSendBlockInfoList(const QList<BlockRealInfo>& list);
    void signalSendTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void signalRecvTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void signalQueryTop10ChinaStockInfos(const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    //历史数据写入数据库
    void signalRecvShareHistoryInfos(const StockDataList& list);
    void signalQueryShareHistoryLastDate(const QString& code);
    void signalSendShareHistoryLastDate(const QString& code, const QDate& date);
public slots:
    void slotRecvShareHistoryInfos(const StockDataList& list);
    bool slotAddHistoryData(const StockData& data);
    void updateBlockInfoList(const QList<BlockRealInfo>& list);
    void addBlock(const BlockRealInfo& info);
    void modBlock(const BlockRealInfo& info);
    void delBlock(int code);
    void queryBlock(int type = 0, bool init = false);
    void recvRealBlockInfo(const QList<BlockRealInfo>& list);
    void slotRecvTop10ChinaStockInfos(const QList<ChinaShareExchange>& list);
    void slotQueryTop10ChinaStockInfos(const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    void slotQueryShareHistoryLastDate(const QString& code);
private:
    void initSignalSlot();
    bool initDatabase();
    bool createHistoryTable(const QString& pTableName);
    bool isTableExist(const QString& pTable);
    bool blockExist(int code);
    bool isActive();
    void initBlockInfo();
    void saveDB();
    QString errMsg();
    //数据库查询指定日期的沪深股通的TOP10交易
    bool queryTop10ChinaShareInfos(QList<ChinaShareExchange>& list, const QDate& date = QDate(), const QString& share = QString(), int market = 0);
    bool addTop10ChinaStockInfo(const ChinaShareExchange& info);
    //查询表的更新日期
    QDate getLastUpdateDateOfTable(const QString& table);

private:    //本类使用的变量
    static HqInfoService *m_pInstance;
    class CGarbo        // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~CGarbo()
        {
            if (HqInfoService::m_pInstance)
            {
                delete HqInfoService::m_pInstance;
                HqInfoService::m_pInstance = NULL;
            }
        }
    };
    static CGarbo s_Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
    QThread             m_threadWork;       //工作线程
    bool                        mDataBaseInitFlag;
    QSqlDatabase                mDB;
    int                         mCurrentRPLIndex;
    QSqlQuery                   mSqlQuery;
    QMap<int, BlockRealInfo>    mBlockInfo;
    QTimer                      *mUpdateTimer;
    QStringList                 mNotExchangeDaysList;
};

#endif // DBSERVICE_H