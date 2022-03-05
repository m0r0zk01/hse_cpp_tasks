#include "admission.h"
#include <algorithm>
#include <tuple>

bool Cmp1(const Applicant* a, const Applicant* b) {
    int a_negative_points = -a->points, b_negative_points = -b->points;
    return std::tie(a_negative_points, a->student.birth_date.year, a->student.birth_date.month,
                    a->student.birth_date.day, a->student.name) <
           std::tie(b_negative_points, b->student.birth_date.year, b->student.birth_date.month,
                    b->student.birth_date.day, b->student.name);
}

bool Cmp2(const Student* a, const Student* b) {
    return std::tie(a->name, a->birth_date.year, a->birth_date.month, a->birth_date.day) <
           std::tie(b->name, b->birth_date.year, b->birth_date.month, b->birth_date.day);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::unordered_map<std::string, int> university_places_left;
    for (const auto& university : universities) {
        university_places_left[university.name] = university.max_students;
    }

    std::vector<const Applicant*> sorted_applicants;
    for (const auto& applicant : applicants) {
        sorted_applicants.push_back(&applicant);
    }

    sort(sorted_applicants.begin(), sorted_applicants.begin(), Cmp1);

    AdmissionTable result;

    for (const Applicant* applicant : sorted_applicants) {
        for (const std::string& university_name : (*applicant).wish_list) {
            auto it = university_places_left.find(university_name);
            if (it->second > 0) {
                --(it->second);
                result[university_name].push_back(&(applicant->student));
                break;
            }
        }
    }

    for (auto& kvp : result) {
        sort(kvp.second.begin(), kvp.second.end(), Cmp2);
    }

    return result;
}
