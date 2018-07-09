#include "gitreference.h"

#include <QDebug>

#include <git2.h>

GitReference::GitReference(git_reference *ref, GitRepository *parent) : GitBaseOid(nullptr, parent)
  ,m_namespace(Invalid)
{
    if (ref == nullptr) {
        qDebug() << "Null reference ptr";
        return;
    }

    m_raw = ref;

    detectNamespace();
    if (m_namespace == Invalid) {
        qDebug() << "Namespace for reference is invalid";
        free();
        return;
    }
}

GitReference::GitReference(GitReference &&other) : GitBaseOid(std::move(other))
{
    free();
    m_raw = other.m_raw;
    m_namespace = other.m_namespace;
    other.m_raw = nullptr;
    other.m_namespace = Invalid;
}

GitReference& GitReference::operator=(GitReference &&other)
{
    if (&other != this) {
        free();
        m_raw = other.m_raw;
        m_namespace = other.m_namespace;
        other.m_raw = nullptr;
        other.m_namespace = Invalid;
    }
    return static_cast<GitReference&>(GitBaseOid::operator=(std::move(other)));
}

void GitReference::detectNamespace()
{
    if (m_raw == nullptr) {
        return;
    }

    if (git_reference_is_branch(m_raw)) {
        m_namespace = Branch;
        qDebug() << "Reference namespace Branch";
        return;
    }

    if (git_reference_is_tag(m_raw)) {
        m_namespace = Tag;
        qDebug() << "Reference namespace Tag";
        return;
    }

    if (git_reference_is_note(m_raw)) {
        m_namespace = Note;
        qDebug() << "Reference namespace Note";
        return;
    }

    if (git_reference_is_remote(m_raw)) {
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
    if (m_raw != nullptr) {
        git_reference_free(m_raw);
    }
    m_raw = nullptr;
}

QString GitReference::refName() const
{
    return QString(git_reference_name(m_raw));
}

