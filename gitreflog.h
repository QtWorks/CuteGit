#ifndef GITREFLOG_H
#define GITREFLOG_H

struct git_reflog;

class GitRepository;

class GitReflog
{
public:
    GitReflog(GitRepository* repo);
private:
    git_reflog* m_reflog;
};

#endif // GITREFLOG_H
