#include "gitreference.h"

#include <QDebug>

#include <git2.h>

GitReference::GitReference(git_reference *ref, GitRepository *parent) : GitBaseOid(nullptr, parent)
  ,m_namespace(Invalid)
{
    if(ref == nullptr) {
        qDebug() << "Null reference ptr";
        return;
    }

    m_raw = ref;

    detectNamespace();
    if(m_namespace == Invalid) {
        qDebug() << "Namespace for reference is invalid";
        free();
        return;
    }
}

void GitReference::detectNamespace()
{
    if(m_raw == nullptr) {
        return;
    }

    if(git_reference_is_branch(m_raw)) {
        m_namespace = Branch;
        qDebug() << "Reference namespace Branch";
        return;
    }

    if(git_reference_is_tag(m_raw)) {
        m_namespace = Tag;
        qDebug() << "Reference namespace Tag";
        return;
    }

    if(git_reference_is_note(m_raw)) {
        m_namespace = Note;
        qDebug() << "Reference namespace Note";
        return;
    }

    if(git_reference_is_remote(m_raw)) {
        m_namespace = Remote;
        qDebug() << "Reference namespace Remote";
        return;
    }
}

GitReference::~GitReference()
{
    free();
}

void GitReference::free()
{
    if(m_raw != nullptr) {
        git_reference_free(m_raw);
    }

    m_raw = nullptr;
}
