#ifndef ROI_ANALYZER_H
#define ROI_ANALYZER_H

#include <string>
#include <utility>
#include <vector>
#include "labels.h"
#include "classifier.h"

class roi_analyzer : public QObject
{   Q_OBJECT
public:
    roi_analyzer(QObject *parent = 0);
    std::vector<Prediction> analyze_frame(cv::Mat image);

signals:
    void update_analyze_result(const QPair<QString, float> &result);

private:
    bool initial_Classifier();
    Classifier *gender_recognizer;
    Classifier *age_recognizer;
};

#endif // ROI_ANALYZER_H
