#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <istream>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <time.h>

#include <QDebug>
#include <QString>
#include <QObject>
#include <QStandardPaths>

class Segmentation : public QObject {
Q_OBJECT
    friend class SegmentationObjectModel;
    friend class TouchedObjectModel;
    friend class SegmentationTab;

    class Object;
    class SubObject {
        friend class Segmentation;
        std::vector<std::reference_wrapper<Object>> objects;
    public:
        const uint64_t id;
        bool selected;
        explicit SubObject(const uint64_t id) : id(id), selected(false) {}
        SubObject(SubObject &&) = delete;
        SubObject(const SubObject &) = delete;
    };

    template<typename T, typename U>
    friend bool operator<(const std::reference_wrapper<T> & lhs, const std::reference_wrapper<U> & rhs) {
        return lhs.get().id < rhs.get().id;
    }
    template<typename T, typename U>
    friend bool operator==(const std::reference_wrapper<T> & lhs, const std::reference_wrapper<U> & rhs) {
        return lhs.get().id == rhs.get().id;
    }

    class Object {
        friend class SegmentationObjectModel;
        friend class TouchedObjectModel;
        friend class Segmentation;

        QString category;

        //see http://coliru.stacked-crooked.com/a/aba85777991b4425
        std::vector<std::reference_wrapper<SubObject>> subobjects;
    public:
        const uint64_t id;
        const bool immutable;
        bool selected = false;
        Object(Object &&) = delete;
        Object(const Object &) = delete;
        explicit Object(SubObject & initialVolume);
        explicit Object(const uint64_t id, const bool & immutable, SubObject & initialVolume);
        explicit Object(const uint64_t id, const bool & immutable, std::vector<std::reference_wrapper<SubObject>> initialVolumes);
        explicit Object(const uint64_t, const Object &, const Object &);
        bool operator==(const Object & other) const;
        void addExistingSubObject(SubObject & sub);
        Object & merge(const Object & other);
        Object & moveFrom(Object & other);
    };

    std::unordered_map<uint64_t, SubObject> subobjects;
    std::unordered_map<uint64_t, Object> objects;
    std::vector<std::reference_wrapper<Object>> selectedObjects;
    // Selection via subobjects touches all objects containing the subobject.
    // The biggest touched object, i.e. the one with the most subobjects, is selected.
    uint64_t touched_subobject_id = 0;
    bool renderAllObjs; // show all segmentations as opposed to only a selected one
    static uint64_t highestObjectId;

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> subobjectColor(const uint64_t subObjectID) const;
    Object & const_merge(Object & one, Object & other);
    Object & merge(Object & one, Object & other);
public:
    static Segmentation & singleton();
    // This array holds the table for overlay coloring.
    // The colors should be "maximally different".
    std::array<std::array<uint8_t, 256>, 3> overlayColorMap = [](){
        std::array<std::array<uint8_t, 256>, 3> lut;
        //interleaved spectral colors
        for (std::size_t i = 0; i < 256; ++i) {
            lut[0][i] = i;
            lut[1][i] = 255;
            lut[2][i] = 0;
            ++i;
            lut[0][i] = i;
            lut[1][i] = 0;
            lut[2][i] = 255;
            ++i;
            lut[0][i] = 255;
            lut[1][i] = i;
            lut[2][i] = 0;
            ++i;
            lut[0][i] = 0;
            lut[1][i] = i;
            lut[2][i] = 255;
            ++i;
            lut[0][i] = 255;
            lut[1][i] = 0;
            lut[2][i] = i;
            ++i;
            lut[0][i] = 0;
            lut[1][i] = 255;
            lut[2][i] = i;
        }
        return lut;
    }();
    uint8_t alpha;
    bool segmentationMode;
    QString filename;

    Segmentation();
    bool hasObjects();
    void setDefaultFilename();
    Object & createObject(const uint64_t initialSubobjectId);
    Object & createObject(const uint64_t objectId, const uint64_t initialSubobjectId, const bool & immutable = false);
    void removeObject(Object &);
    void newSubObject(Object & obj, uint64_t subObjID);
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> subobjectColorUniqueFromId(const uint64_t subObjectID) const;
    uint64_t subobjectIdFromUniqueColor(std::tuple<uint8_t, uint8_t, uint8_t> color) const;
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> objectColorFromSubobject(const uint64_t subObjectID) const;
    bool subobjectExists(const uint64_t & subobjectId);
    SubObject & subobjectFromId(const uint64_t & subobjectId);
    Object & largestObjectContainingSubobject(const SubObject & subobject) const;
    void touchObjects(const uint64_t subobject_id);
    void untouchObjects();
    std::vector<std::reference_wrapper<Object>> touchedObjects();
    bool isSelected(const SubObject & rhs);
    bool isSelected(const Object & rhs);
    void clearObjectSelection();
    void selectObject(Object & object);
    void unselectObject(Object & object);
    void unmergeObject(Object & object, SubObject & subobject);
    void unmergeSubObject(Object & object, SubObject & subobject);
    void selectObjectFromSubObject(SubObject &subobject);
    void selectObject(const uint64_t & objectId);
    std::size_t selectedObjectsCount();
    void saveMergelist(const QString & toFile = Segmentation::singleton().filename);
    void loadMergelist(const std::string & fileName);
signals:
    void dataChanged();
public slots:
    void deleteSelectedObjects();
    void mergeSelectedObjects();
};

#endif // SEGMENTATION_H
