#pragma once
#include "stdafx.h"
#include "databasemanager.h"

using namespace testing;

bool firstWrite = true;

struct DBFixture: public Test
{
    DatabaseManager::DatabaseManager db;
    ClientInfo::ClientInfo UIStruct;
    QByteArray password;
    QString testName;


    DBFixture()
    {
        db.ConnectToDataBase();
        password.append("hello");
        testName = "Test";
        UIStruct.name = "Test";
        UIStruct.surName = "TestTest";
    }

protected:
    virtual void SetUp()
    {
        QSqlQuery qry(db.GetDB());
        qry.prepare("SELECT* FROM Users WHERE Username = 'yegor'");
        qry.exec();
        if(!qry.next())
        {
            qry.prepare("INSERT INTO Users (Username, Password, Name, Surname) Values (:name, :pass, 'yegor', 'yegor');");
            qry.bindValue(":name", "yegor");
            qry.bindValue(":pass", password.toHex());
            qry.exec();
        }
    }
    virtual void TearDown()
    {
        QSqlQuery qry(db.GetDB());

        qry.prepare("DELETE FROM Users WHERE Username = '" + testName + "';");
        qry.exec();
        db.GetDB().close();
    }

};


TEST_F(DBFixture, UsernameBusy)
{
    EXPECT_TRUE(db.IsUsernameBusy("yegor"));
}


TEST_F(DBFixture, UsernameFree)
{
    EXPECT_FALSE(db.IsUsernameBusy(testName));
}


TEST_F(DBFixture, WriteNotExistLogin)
{
    EXPECT_TRUE(db.WriteToDataBase(testName, password, UIStruct));
    QSqlQuery qry(db.GetDB());

    qry.prepare("SELECT* FROM Users WHERE Username = '" + testName + "' ;");
    EXPECT_TRUE(qry.exec());
    EXPECT_TRUE(qry.next());
}


TEST_F(DBFixture, WriteExistLogin)
{
    EXPECT_FALSE(db.WriteToDataBase("yegor", password, UIStruct));
}


TEST_F(DBFixture, LoginTruePasswordTrue)
{
    EXPECT_TRUE(db.IsCorrectLogin("yegor", password));
}

TEST_F(DBFixture, LoginTruePasswordFalse)
{
    EXPECT_FALSE(db.IsCorrectLogin("yegor", "111"));
}

TEST_F(DBFixture, LoginFalsePasswordTrue)
{
    EXPECT_FALSE(db.IsCorrectLogin("yegoryegor", password));
}

TEST_F(DBFixture, LoginFalsePasswordFalse)
{
    EXPECT_FALSE(db.IsCorrectLogin("yegoryegor", "111"));
}

TEST_F(DBFixture, FillMapTest)
{
    QMap<QString, ClientInfo::ClientInfo> testMap;
    QMap<QString, ClientInfo::ClientInfo> resultMap;
    ClientInfo::ClientInfo structClient;
    structClient.name = "yegor";
    structClient.surName = "yegor";
    testMap.insert("yegor", structClient);

    db.FillMapUsername(resultMap);
    EXPECT_EQ(resultMap.begin().key(), testMap.begin().key());
    EXPECT_EQ(resultMap.begin().value().name, testMap.begin().value().name);
    EXPECT_EQ(resultMap.begin().value().surName, testMap.begin().value().surName);
}
