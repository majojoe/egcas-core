#ifndef EGCKERNELPARSER_H
#define EGCKERNELPARSER_H

class EgcNode;

#include <QString>

/**
 * @brief The EgcKernelParser class encapsulates the parsing of the cas kernel output and creates a node tree thereof.
 */
class EgcKernelParser
{
public:
        EgcKernelParser();
        virtual ~EgcKernelParser() {}

        /**
         * @brief parseKernelOutput parse the cas kernel output and generate a expression tree from it.
         * @param strToParse the output of the cas kernel to generate a tree from
         * @return the result (tree) of the parsing of the cas kernel output
         */
        EgcNode* parseKernelOutput(const QString& strToParse);

        /**
         * @brief getErrorMessage the error message if an error while parsing ocurred.
         * @return the error message if one.
         */
        QString getErrorMessage();
private:
        QString m_errMessage;   /// stores a error message if an error ocurred while parsing
};

#endif // EGCKERNELPARSER_H
