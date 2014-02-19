#include "knossos-global.h"
#include "skeletonizer.h"

skeletonState::skeletonState()
{
}

int skeletonState::getSkeletonTime() {
    return skeletonTime;
}

bool skeletonState::hasUnsavedChanges() {
    return unsavedChanges;
}

treeListElement *skeletonState::getFirstTree() {
    return firstTree;
}

QString skeletonState::getSkeletonFile() {
    return skeletonFileAsQString;
}

bool skeletonState::fromXml(QString file) {
   return loadSkeleton(file);
}

bool skeletonState::toXml(QString file) {
    return saveSkeleton(file);
}

void skeletonState::addTree(treeListElement *tree) {
    treeListElement *theTree = Skeletonizer::addTreeListElement(true, CHANGE_MANUAL, tree->treeID, tree->color, false);
    if(tree->comment) {
        Skeletonizer::addTreeComment(CHANGE_MANUAL, tree->treeID, tree->comment);
    }

    Skeletonizer::setActiveTreeByID(theTree->getTreeID());
    emit updateToolsSignal();
    emit treeAddedSignal(theTree);
}

void skeletonState::addTree(int treeID, Color color, QString comment) {
    color4F c4f;
    c4f.r = color.r;
    c4f.g = color.g;
    c4f.b = color.b;
    c4f.a = color.a;


    treeListElement *theTree = Skeletonizer::addTreeListElement(true, CHANGE_MANUAL, treeID, c4f, false);
    if(comment.isNull() == false) {
        Skeletonizer::addTreeComment(CHANGE_MANUAL, treeID, comment.toLocal8Bit().data());
    }

    Skeletonizer::setActiveTreeByID(treeID);
    emit updateToolsSignal();
    emit treeAddedSignal(theTree);
}

void skeletonState::addNode(nodeListElement *node) {
    if(Skeletonizer::addNode(CHANGE_MANUAL, node->nodeID, node->radius, node->getParentID(), &node->position, node->getViewport(), node->getMagnification(), node->getTime(), false, false)) {
        Skeletonizer::setActiveNode(CHANGE_MANUAL, 0, node->nodeID);
        emit nodeAddedSignal();
    }
}

void skeletonState::addNode(int nodeID, float radius, int x, int y, int z, int inVp, int inMag, int time) {
    int activeID = 0;
    if(activeTree) {
        activeID = activeTree->treeID;
    }

    Coordinate coordinate;
    coordinate.x = x;
    coordinate.y = y;
    coordinate.z = z;

    if(Skeletonizer::addNode(CHANGE_MANUAL, nodeID, radius, activeID, &coordinate, inVp, inMag, time, false, false)) {
        Skeletonizer::setActiveNode(CHANGE_MANUAL, activeNode, nodeID);
        emit nodeAddedSignal();
    }

}

void skeletonState::addNode(int nodeID, int radius, int parentID, int x, int y, int z, int inVp, int inMag, int time) {
    Coordinate coordinate;
    coordinate.x = x;
    coordinate.y = y;
    coordinate.z = z;

    if(Skeletonizer::addNode(CHANGE_MANUAL, nodeID, radius, parentID, &coordinate, inVp, inMag, time, false, false)) {
        Skeletonizer::setActiveNode(CHANGE_MANUAL, activeNode, nodeID);
        emit nodeAddedSignal();
    }
}

void skeletonState::addNode(int x, int y, int z, int viewport) {
    Coordinate coordinate;
    coordinate.x = x;
    coordinate.y = y;
    coordinate.z = z;

    if(viewport < VIEWPORT_XY | viewport > VIEWPORT_ARBITRARY) {
        qDebug() << "viewport is out of range";
        return;
    }

    emit addNodeSignal(&coordinate, viewport);
    emit nodeAddedSignal();
}

QList<treeListElement *> *skeletonState::getTrees() {
    QList<treeListElement *> *trees = new QList<treeListElement *>();
    treeListElement *currentTree = firstTree;
    while(currentTree) {
        trees->append(currentTree);
        currentTree = currentTree->next;
    }
    return trees;
}

void skeletonState::addTrees(QList<treeListElement *> list) {
    for(int i = 0; i < list.size(); i++) {
        treeListElement *currentTree = list.at(i);
        Skeletonizer::addTreeListElement(true, CHANGE_MANUAL, currentTree->treeID, currentTree->color, false);
        emit treeAddedSignal(currentTree);
        if(currentTree->comment) {
            Skeletonizer::addTreeComment(CHANGE_MANUAL, currentTree->treeID, currentTree->comment);
        }

    }
}

bool  skeletonState::deleteTree(int id) {
   return Skeletonizer::delTree(CHANGE_MANUAL, id, true);
}
