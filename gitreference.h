#ifndef GITREFERENCE_H
#define GITREFERENCE_H

#include <gitbaseoid.h>

class GitRepository;
struct git_reference;

class GitReference : public GitBaseOid<git_reference>
{
    Q_OBJECT
    Q_PROPERTY(ReferenceNamespace referenceNamespace READ referenceNamespace CONSTANT)
    Q_ENUMS(ReferenceNamespace)

public:
    enum ReferenceNamespace {
        Invalid,
        Branch,
        Note,
        Tag,
        Remote
    };

    virtual ~GitReference();

    ReferenceNamespace referenceNamespace() const
    {
        return m_namespace;
    }

protected:
    GitReference(git_reference* ref, GitRepository* parent);
    void free();

    ReferenceNamespace m_namespace;

private:
    GitReference();
    Q_DISABLE_COPY(GitReference)
    void detectNamespace();
};

#endif // GITREFERENCE_H
