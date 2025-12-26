#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QFile>
#include "prizesettingdialog.h"
#include <QMessageBox>
#include "xlsxdocument.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setFullBackground();
    ui->joinCountLabel->hide();
    m_clickTimer.invalidate(); // 初始化计时器为无效状态
    m_settingsClickCount = 0;
    this->showMaximized();
    this->setWindowIcon(QIcon(":/img/logo.ico"));
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Widget::onTimerTimeout);
    QString path = QCoreApplication::applicationDirPath() + "/prizes.json";
    qDebug() << "正在尝试加载配置：" << path; // 运行后看控制台输出，确认路径是否正确
    loadPrizeConfig(path);

    if (m_prizes.isEmpty()) {
        qDebug() << "警告：无奖项配置，主流程已拦截。";
        ui->startDrawButton->setEnabled(false);
        ui->startDrawButton->setText("请先设置奖项");
        return;
    }

    QString csvPath =
        QDir(QCoreApplication::applicationDirPath())
            .filePath("people.csv");
    QString excelPath =
        QDir(QCoreApplication::applicationDirPath())
            .filePath("people.xlsx");
    //loadPeopleCsv(csvPath);
    loadPeopleExcel(excelPath);



    loadHistoryFromTxt();

    updatePrizeUI();
    updateRoundUI();
}

Widget::~Widget()
{
    delete ui;
}


// 1. 将内存数据写入 prizes.json
void Widget::savePrizeConfigToJson() {
    QJsonArray array;
    for (const auto &c : m_prizes) {
        QJsonObject obj;
        obj["id"] = c.id;
        obj["displayName"] = c.displayName;
        obj["priceName"] = c.priceName;
        obj["image"] = c.image;
        obj["winnersPerRound"] = c.winnersPerRound;
        obj["totalRounds"] = c.totalRounds;
        obj["allowUsedPeople"] = c.allowUsedPeople; // 存入 JSON
        array.append(obj);
    }

    QFile file(QCoreApplication::applicationDirPath() + "/prizes.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(array).toJson());
        file.close();
    }
}

// 2. 设置按钮点击事件
void Widget::on_btnSettings_clicked()
{
    // 1. 检查时间间隔：如果两次点击超过 1 秒，重新计数
    if (!m_clickTimer.isValid() || m_clickTimer.elapsed() > 1000) {
        m_settingsClickCount = 1;
    } else {
        m_settingsClickCount++;
    }

    // 每次点击后重写计时器
    m_clickTimer.restart();

    // 2. 检查是否达到 5 次
    if (m_settingsClickCount >= 5) {
        // 重置计数器，防止下次进来又是 1 次就开
        m_settingsClickCount = 0;
        m_clickTimer.invalidate();

        // --- 这里写你原本打开设置界面的代码 ---
        PrizeSettingDialog dialog(m_prizes, this);
        if (dialog.exec() == QDialog::Accepted) {
            m_prizes = dialog.getNewConfigs();

            updatePrizeUI();
        }
    } else {
        // 可选：在控制台或者状态栏小提示，调试用，正式发布可以删掉
        qDebug() << "还要再点" << (5 - m_settingsClickCount) << "次打开配置";
    }
}

void Widget::setFullBackground()
{
    QPixmap bg(":/img/bg.jpg");
    if (bg.isNull()) {
        return;
    }

    QPixmap scaled = bg.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(scaled));  // 用 Window，兼容所有 Qt5/Qt6
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

void Widget::loadPeopleCsv(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开 CSV 文件:" << path;
        // 如果找不到文件，显示一个友好的提示
        ui->joinCountLabel->setText("参与人数: 0 (未找到people.csv)");
        return;
    }

    QTextStream in(&file);
    // Qt 6 默认 UTF-8，无需 setCodec

    m_people.clear();
    bool isFirstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        QStringList parts = line.split(",");
        if (parts.size() >= 2) {
            Person p;
            p.employeeId = parts.at(0).trimmed();
            p.name = parts.at(1).trimmed();
            m_people.append(p);
        }
    }
    file.close();

    // 加载完后更新界面上的统计数字
    ui->joinCountLabel->setText(QString("参与人数: %1").arg(m_people.size()));
}

void Widget::loadPeopleExcel(const QString &path) {
    QXlsx::Document xlsx(path);
    if (!xlsx.load()) {
        qWarning() << "读取 Excel 失败，请检查路径:" << path;
        ui->joinCountLabel->setText("参与人数: 0 (Excel文件加载失败)");
        return;
    }

    m_people.clear();
    int row = 2; // 默认第一行是：工号, 姓名, 部门...

    while (true) {
        // 读取第一列和第二列
        QVariant idVar = xlsx.read(row, 1);
        QVariant nameVar = xlsx.read(row, 2);

        // 如果工号那一格是空的，说明读到表格末尾了
        if (idVar.isNull() || idVar.toString().trimmed().isEmpty()) {
            break;
        }

        Person p;
        p.employeeId = idVar.toString().trimmed();
        // 如果有工号但没填姓名，给个“未知”占位
        p.name = nameVar.isNull() ? "未知" : nameVar.toString().trimmed();

        m_people.append(p);
        row++;
    }

    // 更新界面：这里非常关键，只要 m_people 加载进来了，
    // 后面的 loadHistoryFromTxt 就会自动对比黑名单
    ui->joinCountLabel->setText(QString("参与人数: %1").arg(m_people.size()));
}

void Widget::loadPrizeConfig(const QString &path)
{
    QFile file(path);
    m_prizes.clear(); // 清空当前内存数据

    if (!file.exists()) {
        qDebug() << "未发现配置文件，等待用户手动配置。";
        // 如果没有配置，我们给一个“虚拟”的提示状态，或者直接留空
        updatePrizeUI();
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) return;

    QJsonArray prizeArray = doc.array();
    for (const QJsonValue &value : prizeArray) {
        QJsonObject obj = value.toObject();
        PrizeConfig config;
        config.id = obj["id"].toInt();
        config.displayName = obj["displayName"].toString();
        config.priceName = obj["priceName"].toString();
        config.image = obj["image"].toString();
        config.winnersPerRound = obj["winnersPerRound"].toInt();
        config.totalRounds = obj["totalRounds"].toInt();
        config.allowUsedPeople = obj["allowUsedPeople"].toBool();
        m_prizes.append(config);
    }

    // 依然保持 ID 排序
    std::sort(m_prizes.begin(), m_prizes.end(), [](const PrizeConfig &a, const PrizeConfig &b) {
        return a.id < b.id;
    });

    m_currentPrizeIndex = 0;
    updatePrizeUI();
}

void Widget::updatePrizeUI()
{
    if (m_currentPrizeIndex < 0 || m_currentPrizeIndex >= m_prizes.size()) return;
    const PrizeConfig &current = m_prizes[m_currentPrizeIndex];

    ui->prizeLevelLabel->setText(current.displayName); // 显示“一等奖”
    ui->prizeNameLabel->setText(current.priceName);    // 显示“苹果手机”

    // 3. 更新图片
    QString fullPath = QDir(QCoreApplication::applicationDirPath()).filePath(current.image);
    QPixmap pix(fullPath);
    if (!pix.isNull()) {
        ui->giftLabel->setPixmap(pix.scaled(ui->giftLabel->size(),
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation));
        ui->giftLabel->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "图片加载失败:" << fullPath;
        ui->giftLabel->setText("（未找到奖品图片）");
    }
}

void Widget::updateRoundUI() {
    if (m_currentPrizeIndex < 0 || m_currentPrizeIndex >= m_prizes.size()) return;

    const PrizeConfig &current = m_prizes[m_currentPrizeIndex];

    // 假设你的控件名叫 roundLabel
    // 显示效果如：第 1 / 3 轮
    QString text = QString("第 %1 / %2 轮")
                       .arg(m_currentRound)
                       .arg(current.totalRounds);

    ui->roundLabel->setText(text);

    // 如果想要样式好看点，可以在代码里加点颜色
    //ui->roundLabel->setStyleSheet("color: #FFD700; font-weight: bold; font-size: 18px;");
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    setFullBackground();
}

void Widget::onTimerTimeout()
{
    if (m_currentPrizeIndex < 0) return;
    const PrizeConfig &currentPrize = m_prizes[m_currentPrizeIndex];

    // 1. 准备实时滚动池
    QVector<Person> pool;
    QSet<QString> peopleWonInThisPrize = getIdsFromCurrentSection(currentPrize);
    for (const Person &p : m_people) {
        if (currentPrize.allowUsedPeople) {
            if (!peopleWonInThisPrize.contains(p.employeeId)) pool.append(p);
        } else {
            if (!m_usedPeople.contains(p.employeeId)) pool.append(p);
        }
    }

    if (pool.isEmpty()) return;

    // 2. 计算实际能滚动的框
    int actualScrollCount = qMin((int)m_dynamicLabels.size(), pool.size());

    for (int i = 0; i < m_dynamicLabels.size(); ++i) {
        if (i < actualScrollCount) {
            m_dynamicLabels[i]->show();
            int randomIdx = QRandomGenerator::global()->bounded(pool.size());
            Person p = pool[randomIdx];
            m_dynamicLabels[i]->setText(QString("%1\n%2").arg(p.name).arg(p.employeeId));
            // 滚动时的样式也要带上字体大小
            m_dynamicLabels[i]->setStyleSheet("font-size: 28px; font-weight: bold; color: #333333; background-color: rgba(255, 255, 255, 200); border-radius: 10px;");
        } else {
            // 多余的框在滚动阶段就直接隐藏
            m_dynamicLabels[i]->hide();
        }
    }
}

// 核心逻辑 A：确定最终名单
void Widget::drawFinalWinners()
{
    if (m_currentPrizeIndex < 0 || m_currentPrizeIndex >= m_prizes.size()) return;
    const PrizeConfig &currentPrize = m_prizes[m_currentPrizeIndex];

    // 1. 筛选当前池子
    QVector<Person> pool;
    QSet<QString> peopleWonInThisPrize = getIdsFromCurrentSection(currentPrize);
    for (const Person &p : m_people) {
        if (currentPrize.allowUsedPeople) {
            if (!peopleWonInThisPrize.contains(p.employeeId)) pool.append(p);
        } else {
            if (!m_usedPeople.contains(p.employeeId)) pool.append(p);
        }
    }

    // 2. 【核心逻辑】计算实际中奖人数
    // 假设界面有15个框(m_dynamicLabels.size)，但池子只有9人(pool.size)
    int actualCount = qMin((int)m_dynamicLabels.size(), pool.size());

    QVector<Person> winners;
    // 3. 遍历所有的 Label
    for (int i = 0; i < m_dynamicLabels.size(); ++i) {
        if (i < actualCount) {
            // 这位是中奖的
            int r = QRandomGenerator::global()->bounded(pool.size());
            Person winner = pool.takeAt(r);

            // 如果不是全员抽，才加入全局黑名单
            if (!currentPrize.allowUsedPeople) {
                m_usedPeople.insert(winner.employeeId);
            }
            winners.append(winner);

            // 更新 UI，并强制固定字体大小
            m_dynamicLabels[i]->show();
            m_dynamicLabels[i]->setText(QString("%1\n%2").arg(winner.name).arg(winner.employeeId));
            m_dynamicLabels[i]->setStyleSheet(
                "background-color: #FFFACD; "
                "border: 3px solid #E60000; "
                "border-radius: 10px; "
                "font-size: 28px; "  // 强制大字体
                "font-weight: bold; "
                "color: #E60000; "
                "min-height: 100px;"
                );
        } else {
            // 【核心修复】多出来的框，直接隐藏
            m_dynamicLabels[i]->hide();
        }
    }

    // 4. 同步到硬盘
    if (!winners.isEmpty()) {
        saveWinnersToTxt(currentPrize.displayName, winners);
    }
}

QSet<QString> Widget::getIdsFromCurrentSection(const PrizeConfig &config)
{
    QSet<QString> ids;
    QString filePath = QCoreApplication::applicationDirPath() + "/winners_config.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ids;

    // --- 修改点：构造唯一的识别标签 ---
    // 组合：[等级-奖品名] 或 [等级-奖品名(All)]
    QString target = QString("%1-%2").arg(config.displayName, config.priceName);
    if (config.allowUsedPeople) target += "(All)";
    QString bracketTitle = "[" + target + "]";

    QTextStream in(&file);
    bool inTargetPrize = false;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // --- 修改点：匹配组合后的标签 ---
        if (line == bracketTitle) {
            inTargetPrize = true;
            continue;
        }
        // 遇到下一个 [ 说明当前奖项段落结束
        if (line.startsWith("[") && line != bracketTitle) {
            inTargetPrize = false;
        }

        if (inTargetPrize) {
            QString id = line.split(" ").at(0);
            if (!id.isEmpty()) ids.insert(id);
        }
    }
    file.close();
    return ids;
}

// 核心逻辑 B：处理轮数切换
void Widget::processRoundLogic()
{
    const PrizeConfig &current = m_prizes[m_currentPrizeIndex];

    if (m_currentRound < current.totalRounds) {
        // 进入当前奖项的下一轮
        m_currentRound++;
    } else {
        // 当前奖项抽完了，尝试进入下一个奖项
        if (m_currentPrizeIndex < m_prizes.size() - 1) {
            m_currentPrizeIndex++;
            m_currentRound = 1; // 新奖项从第一轮开始
            updatePrizeUI();    // 切换奖品图片
        } else {
            // 所有奖项都抽完了
            ui->startDrawButton->setEnabled(false);
            ui->startDrawButton->setText("抽奖结束");
            return;
        }
    }
    updateRoundUI(); // 更新界面上的 "第 X/Y 轮"
}

void Widget::saveWinnersToTxt(const QString &prizeName, const QVector<Person> &winners)
{
    QString filePath = QCoreApplication::applicationDirPath() + "/winners_config.txt";
    QFile file(filePath);

    // --- 修改点 1：获取当前奖项配置，构造唯一标识 ---
    const PrizeConfig &current = m_prizes[m_currentPrizeIndex];

    // 组合：等级-奖品名
    QString finalTitle = QString("%1-%2").arg(current.displayName, current.priceName);
    if (current.allowUsedPeople) {
        finalTitle += "(All)";
    }

    // --- 修改点 2：检查这个“组合标签”是否已存在 ---
    bool titleExists = false;
    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString content = file.readAll();
        if(content.contains("[" + finalTitle + "]")) titleExists = true;
        file.close();
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        QTextStream out(&file);
        // 如果是该奖项的第一轮，写入 [等级-奖品名] 标签
        if(!titleExists) {
            out << "\n[" << finalTitle << "]\n";
        }

        for(const Person &p : winners){
            out << p.employeeId << " " << p.name << "\n";
        }
        file.close();
    }
}

// 恢复：启动时自动读档，填入 m_usedPeople
void Widget::loadHistoryFromTxt() {
    QString filePath = QCoreApplication::applicationDirPath() + "/winners_config.txt";
    QFile file(filePath);
    m_usedPeople.clear();

    if (m_prizes.isEmpty()) return;

    QMap<QString, int> prizeCountMap;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString currentSection = "";
        bool isAllMemberSection = false; // 标识当前段落是否为“全员抽奖”

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            // --- 修改点 1：解析增强后的标签 ---
            if (line.startsWith("[") && line.endsWith("]")) {
                currentSection = line.mid(1, line.length() - 2);
                // 判断标签是否以 (All) 结尾
                isAllMemberSection = currentSection.endsWith("(All)");
            } else {
                QString empId = line.split(" ").at(0);

                // --- 修改点 2：根据标识位决定是否加入全局黑名单 ---
                // 如果当前段落带 (All)，说明是全员抽奖，不加进 m_usedPeople
                if (!isAllMemberSection) {
                    m_usedPeople.insert(empId);
                }

                // 记录该奖项已中奖人数，用于恢复轮数进度
                prizeCountMap[currentSection]++;
            }
        }
        file.close();
    }

    // --- 修改点 3：匹配进度时使用“等级-奖品”组合键 ---
    bool found = false;
    for (int i = 0; i < m_prizes.size(); ++i) {
        const auto &config = m_prizes[i];
        QString key = QString("%1-%2").arg(config.displayName, config.priceName);
        if (config.allowUsedPeople) key += "(All)";

        int drawn = prizeCountMap.value(key, 0);
        int totalNeed = config.totalRounds * config.winnersPerRound;

        if (drawn < totalNeed) {
            m_currentPrizeIndex = i;
            m_currentRound = (drawn / config.winnersPerRound) + 1;
            found = true;
            break;
        }
    }

    // (剩余 UI 更新代码保持不变)
    if (!found) {
        m_currentPrizeIndex = 0;
        m_currentRound = 1;
    }
    ui->startDrawButton->setEnabled(true);
    ui->startDrawButton->setText("开始抽奖");
    updatePrizeUI();
    updateRoundUI();
}

void Widget::on_startDrawButton_clicked()
{
    // 辅助逻辑：获取当前奖项真正的可用人数
    auto getAvailablePoolSize = [&]() -> int {
        if (m_currentPrizeIndex < 0 || m_currentPrizeIndex >= m_prizes.size()) return 0;
        const PrizeConfig &current = m_prizes[m_currentPrizeIndex];

        int count = 0;
        // 【核心修改】：传入完整的 current 对象，匹配“等级-奖品名”分组逻辑
        QSet<QString> peopleWonInThisPrize = getIdsFromCurrentSection(current);

        for (const Person &p : m_people) {
            if (current.allowUsedPeople) {
                // 全员抽：只要在本奖项（特定等级和奖品）没中过就算可用
                if (!peopleWonInThisPrize.contains(p.employeeId)) count++;
            } else {
                // 普通抽：必须全局没中过才算可用
                if (!m_usedPeople.contains(p.employeeId)) count++;
            }
        }
        return count;
    };

    // 情况 A：正在等待用户确认“切换到下一个奖项”
    if (m_isPendingPrizeSwitch) {
        m_isPendingPrizeSwitch = false;
        m_currentPrizeIndex++;
        m_currentRound = 1;

        updatePrizeUI();
        updateRoundUI();

        qDeleteAll(m_dynamicLabels);
        m_dynamicLabels.clear();

        // 【新增】：重置标题为“候选名单”
        ui->rightTitle->setText("候选名单");

        // 切换后检查新奖项是否还有人可抽
        if (getAvailablePoolSize() <= 0) {
            ui->startDrawButton->setText("人员不足以开启新奖项");
            ui->startDrawButton->setEnabled(false);
        } else {
            ui->startDrawButton->setText("开始抽奖");
        }
        return;
    }

    // 情况 B：正在滚动中 -> 点击停止
    if (m_isRunning) {
        m_isRunning = false;
        m_timer->stop();

        // 【新增】：停止时切换标题
        ui->rightTitle->setText("中奖名单");

        drawFinalWinners(); // 内部已包含隐藏多余框和设置字体的逻辑

        const PrizeConfig &current = m_prizes[m_currentPrizeIndex];
        int availableAfterDraw = getAvailablePoolSize();

        // 1. 先检查是否达到了配置的奖项总轮数
        if (m_currentRound >= current.totalRounds) {
            if (m_currentPrizeIndex < m_prizes.size() - 1) {
                m_isPendingPrizeSwitch = true;
                QString nextPrizeName = m_prizes[m_currentPrizeIndex + 1].displayName;
                ui->startDrawButton->setText("切换至：" + nextPrizeName);
            } else {
                ui->startDrawButton->setText("全部抽奖结束");
                ui->startDrawButton->setEnabled(false);
            }
        }
        // 2. 如果轮数还没完，但池子已经空了
        else if (availableAfterDraw <= 0) {
            ui->startDrawButton->setText("人员已抽完");
            ui->startDrawButton->setEnabled(false);
        }
        // 3. 还有人，准备下一轮
        else {
            m_currentRound++;
            if (availableAfterDraw < current.winnersPerRound) {
                ui->startDrawButton->setText(QString("准备下轮(剩%1人)").arg(availableAfterDraw));
            } else {
                ui->startDrawButton->setText("准备下一轮");
            }
        }

        // 更新左下角参与人数标签
        int globalUnused = 0;
        for (const Person &p : m_people) if (!m_usedPeople.contains(p.employeeId)) globalUnused++;
        ui->joinCountLabel->setText(QString("参与人数: %1").arg(globalUnused));
        return;
    }

    // 情况 C：静止状态 -> 点击开始
    if (!m_isRunning) {
        int availableCount = getAvailablePoolSize();

        if (availableCount <= 0) {
            ui->startDrawButton->setText("人员不足");
            ui->startDrawButton->setEnabled(false);
            return;
        }

        // 【新增】：开始抽奖时切换标题
        ui->rightTitle->setText("候选名单");

        // 清理上一轮的残余名牌
        if (ui->startDrawButton->text().contains("准备下一轮") || ui->startDrawButton->text().contains("准备下轮")) {
            qDeleteAll(m_dynamicLabels);
            m_dynamicLabels.clear();
            updateRoundUI();
        }

        m_isRunning = true;
        ui->startDrawButton->setText("停止");

        prepareDynamicLabels(availableCount);

        m_timer->start(50);
    }
}

void Widget::prepareDynamicLabels(int availableCount)
{
    // 1. 清理上一轮的残余控件（非常重要，防止重叠和闪退）
    if (ui->groupBox->layout()) {
        QLayoutItem *child;
        while ((child = ui->groupBox->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }
    } else {
        ui->groupBox->setLayout(new QGridLayout());
    }

    m_dynamicLabels.clear();

    // 2. 获取布局并设置间距
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->groupBox->layout());
    layout->setSpacing(15);

    // 3. 根据当前奖项的每轮人数创建 Label
    int count = m_prizes[m_currentPrizeIndex].winnersPerRound;

    for (int i = 0; i < count; ++i) {
        QLabel *label = new QLabel("等待开奖", this);
        label->setAlignment(Qt::AlignCenter);

        // 设置名牌样式
        label->setStyleSheet(
            "QLabel {"
            "  background-color: rgba(255, 255, 255, 220);"
            "  border: 2px solid #FFD700;"
            "  border-radius: 10px;"
            "  font-size: 24px;"
            "  font-weight: bold;"
            "  color: #333333;"
            "  min-height: 100px;"
            "}"
            );

        // 每行最多3个的计算逻辑
        layout->addWidget(label, i / 3, i % 3);
        m_dynamicLabels.append(label);
    }
}
