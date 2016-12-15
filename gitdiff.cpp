#include "gitdiff.h"

#include <gitrepository.h>

#include <QDebug>

#include <git2/types.h>
#include <git2/diff.h>
#include <git2/commit.h>

GitDiff::GitDiff(git_commit* a, git_commit* b, GitRepository *repository) : QObject()
  ,m_repository(repository)
{
    Q_ASSERT_X(m_repository, "GitDiff", "Repository of NULL");
    connect(m_repository, &GitRepository::destroyed, this, &GitDiff::deleteLater);
    readBody(a, b);
}

void GitDiff::readBody(git_commit *a, git_commit *b)
{
    git_diff *diff = nullptr;

    git_tree *a_tree = nullptr;
    git_tree *b_tree = nullptr;


    git_commit_tree(&a_tree, a);
    git_commit_tree(&b_tree, b);

    git_diff_tree_to_tree(&diff, m_repository->raw(), a_tree, b_tree, nullptr);


    git_diff_print(diff,
                   GIT_DIFF_FORMAT_PATCH,
                   [](const git_diff_delta *delta, const git_diff_hunk *hunk,
                   const git_diff_line *line, void *payload)
    {
        Q_UNUSED(hunk)

        QString prefix("<font color=\"%1\">%2");
        QString suffix("</font><br/>");
        GitDiff* diff = static_cast<GitDiff*>(payload);
        QString fileName(delta->new_file.path);
        if(line->origin == GIT_DIFF_LINE_FILE_HDR) {
            return 0;
        }

        switch(line->origin) {
        case GIT_DIFF_LINE_ADDITION:
            prefix = prefix.arg("#00ff00");
            break;
        case GIT_DIFF_LINE_DELETION:
            prefix = prefix.arg("#ff0000");
            break;
        case GIT_DIFF_LINE_HUNK_HDR:
            prefix = "<br/><b>";
            suffix = "</b><br/>";
//            prefix = prefix.arg("#000000").arg("");
            break;
        default:
            prefix = prefix.arg("#000000").arg("&nbsp;");
            break;
        }

        if ( line->origin == GIT_DIFF_LINE_ADDITION ||
                line->origin == GIT_DIFF_LINE_DELETION) {
            prefix = prefix.arg(line->origin);
        }

        diff->m_diffList[fileName].append(prefix);
        diff->m_diffList[fileName].append(QString::fromUtf8(line->content, line->content_len).toHtmlEscaped().replace(" ", "&nbsp;"));
        diff->m_diffList[fileName].append(suffix);
        return 0;
    }, this);

    git_diff_free(diff);
    git_tree_free(a_tree);
    git_tree_free(b_tree);
}

void GitDiff::reset()
{
    m_diffList.clear();
}

QStringList GitDiff::files()
{
    return m_diffList.keys();
}

QString GitDiff::unified(const QString& file)
{
    return m_diffList.value(file);
}
