#include "treelistdecorator.h"

TreeListDecorator::TreeListDecorator(QObject *parent) :
    QObject(parent)
{
}

treeListElement *TreeListDecorator::new_treeListElement() {
    return new treeListElement();
}

treeListElement *TreeListDecorator::new_treeListElement(int treeID, Color color, QString comment) {
    return new treeListElement(treeID, color, comment);
}

treeListElement *TreeListDecorator::new_treeListElement(int treeID, float r, float g, float b, float a, QString comment) {
    return new treeListElement(treeID, r, g, b, a, comment);
}

int TreeListDecorator::getTreeID(treeListElement *self) {
    return self->getTreeID();
}

void TreeListDecorator::setTreeID(treeListElement *self, int treeID) {
    self->setTreeID(treeID);
}

nodeListElement *TreeListDecorator::getRoot(treeListElement *self) {
    return self->getRoot();
}

QList<nodeListElement *> *TreeListDecorator::getNodes(treeListElement *self) {
    return self->getNodes();
}

/*
char *TreeListDecorator::comment(treeListElement *self) {
    return self->comment;
}
*/

/*
void TreeListDecorator::setComment(treeListElement *self, char *comment) {
    if(strlen(comment) < 8192) {
        strcpy(self->comment, comment);
    }
}



*/