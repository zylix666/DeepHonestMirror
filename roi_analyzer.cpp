#include "roi_analyzer.h"

roi_analyzer::roi_analyzer(QObject *parent)
{
    initial_Classifier();
}

bool roi_analyzer::initial_Classifier(){
    bool ret = false;
    gender_recognizer = new Classifier("./models/gender/deploy_gender.prototxt", \
                                       "./models/gender/gender_net.caffemodel", \
                                       "./models/gender/mean.binaryproto",       \
                                       "./models/gender/taglist.txt");
    age_recognizer = new Classifier("./models/age/deploy_age.prototxt", \
                                    "./models/age/age_net.caffemodel", \
                                    "./models/age/mean.binaryproto",       \
                                    "./models/age/taglist.txt");
    return ret;
}

std::vector<Prediction> roi_analyzer::analyze_frame(cv::Mat image){
    std::vector<Prediction> final_result;
    std::vector<Prediction> gender_predictions = gender_recognizer->Classify(image);
    /* Print the top N predictions. */
    final_result.push_back(gender_predictions[0]);
    for (size_t i = 0; i < gender_predictions.size(); ++i) {
      Prediction p = gender_predictions[i];
      std::cout << "Gender : " << std::fixed << std::setprecision(4) << p.second << " - \""
                << p.first << "\"" << std::endl;
    }

    std::vector<Prediction> age_predictions = age_recognizer->Classify(image);
    final_result.push_back(age_predictions[0]);
    /* Print the top N predictions. */
    for (size_t i = 0; i < age_predictions.size(); ++i) {
      Prediction p = age_predictions[i];
      std::cout << "Age : " << std::fixed << std::setprecision(4) << p.second << " - \""
                << p.first << "\"" << std::endl;
    }
    return final_result;
}
