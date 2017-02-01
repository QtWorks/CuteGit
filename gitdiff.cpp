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

GitDiff::~GitDiff()
{
    reset();
}

void GitDiff::readBody(git_commit *a, git_commit *b)
{
    git_diff *diff = nullptr;

    git_tree *aTree = nullptr;
    git_tree *bTree = nullptr;

    git_diff_find_options similarityOpts;

    if(a != nullptr) {
        git_commit_tree(&aTree, a);
    }

    if(b != nullptr) {
        git_commit_tree(&bTree, b);
    }

    git_diff_tree_to_tree(&diff, m_repository->raw(), aTree, bTree, nullptr);

    git_diff_find_init_options(&similarityOpts, GIT_DIFF_FIND_OPTIONS_VERSION);
    git_diff_find_similar(diff, &similarityOpts);

    git_diff_print(diff,
                   GIT_DIFF_FORMAT_PATCH,
                   [](const git_diff_delta *delta, const git_diff_hunk *hunk,
                   const git_diff_line *line, void *payload) -> int
    {
        Q_UNUSED(hunk)

        QString prefix("<font color=\"%1\">%2");
        QString suffix("</font><br/>");
        GitDiff* diff = static_cast<GitDiff*>(payload);
        QString newFileName(delta->new_file.path);
        QString oldFileName(delta->old_file.path);

        QString diffData;

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
            break;
        default:
            prefix = prefix.arg("#000000").arg("&nbsp;");
            break;
        }

        if ( line->origin == GIT_DIFF_LINE_ADDITION ||
                line->origin == GIT_DIFF_LINE_DELETION) {
            prefix = prefix.arg(line->origin);
        }

        if(!diff->m_diffList.contains(newFileName)) {
            diff->m_diffList.insert(newFileName, QPointer<DiffModel>(new DiffModel(newFileName, diff)));
        }

        if(line->origin != GIT_DIFF_LINE_FILE_HDR) { //Add line only in case if origin is not file header
            diffData = QString::fromUtf8(line->content, line->content_len);
        } else if(delta->status == GIT_DELTA_RENAMED){ //else check the similarity
            if(delta->similarity == 100) {
                diffData = QString(oldFileName + " -> " + newFileName);
            }
            diff->m_diffList[newFileName]->setSimilarity(delta->similarity);
        }

        if(!diffData.isEmpty()) {
            diff->m_diffList[newFileName]->append(prefix);
            diff->m_diffList[newFileName]->append(diffData.toHtmlEscaped().replace(" ", "&nbsp;"));
            diff->m_diffList[newFileName]->append(suffix);
        }
        diff->m_diffList[newFileName]->setType(delta->status);
        return 0;
    }, this);

    git_diff_free(diff);
    git_tree_free(aTree);
    git_tree_free(bTree);
}

void GitDiff::reset()
{
    foreach (QPointer<DiffModel> model, m_diffList) {
        model.clear();
    }
    m_diffList.clear();
}

QStringList GitDiff::files()
{
    return m_diffList.keys();
}

DiffModel* GitDiff::model(const QString& file)
{
    DiffModel* model = m_diffList.value(file).data();
    return model;
}
