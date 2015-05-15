#ifndef EGCKERNELCONN_H
#define EGCKERNELCONN_H

#include <QString>
#include <QProcess>
#include <QRegularExpression>

/**
 * @brief The EgcNodeType enum is a enum to differentiate the different node types
 */
enum class EgcKernelStart
{
        beforeStart = 0, Starting, Started
};

/**
 * @brief The EgcKernelConn class is a base class for connections to different CAS kernels
 */
class EgcKernelConn: public QObject
{
        Q_OBJECT
public:
        /**
         * @brief EgcKernelConn creates a connection to a CAS kernel. This is done by starting the kernel.
         * It waits for commands then and will bring back the results.
         * @param executeCmd the path to the maxima executable including arguments.
         */
        EgcKernelConn(QString executeCmd, QObject *parent = 0);
        ///destructor
        virtual ~EgcKernelConn();
        /**
         * @brief sendCommand send a command to the kernel
         * @param cmd command to be sent to the kernel
         */
        virtual void sendCommand(QString cmd);
        /**
         * @brief quit quit kernel subprocess
         *
         * To avoid just killing the kernel you should call quit and wait for the kernelTerminated(); signal
         */
        virtual void quit(void) = 0;
        /**
         * @brief reset resets all variables assinged in the CAS kernel.
         */
        virtual void reset() = 0;

signals:
        /**
         * @brief resultReceived
         * @param result
         */
        void resultReceived(QString result);
        void errorReceived(QString errorMsg);
        void kernelStarted(void);
        void kernelTerminated(void);
        void kernelErrorOccurred(QProcess::ProcessError error);
protected slots:
        virtual void stdOutput(void) = 0;
        virtual void errorOutput(void);
        void kernelTerm(void);
        void kernelError(QProcess::ProcessError error);

protected:
        QString m_result;                       ///< stores the result until the result is complete
        QString m_error;                        ///< stores the error message until the message is complete
        QProcess *m_casKernelProcess;           ///< pointer to the kernel QProcess object
        EgcKernelStart m_startState;            ///< marks the starting state of the CAS kernel
        QRegularExpression m_startRegex;        ///< regex for filtering kernel output at startup
        QRegularExpression m_errRegex;          ///< regex for filtering kernel errors
        QRegularExpression m_regex;             ///< regex for filtering regular kernel output
};

#endif // EGCKERNELCONN_H
