#pragma once
#include <tools/NamedObject.hpp>

class StoreyModel;
class StoreyView;
namespace iscore
{
	class SerializableCommand;
}
class ProcessPresenterInterface;
class ProcessViewModelInterface;
class StoreyPresenter : public NamedObject
{
	Q_OBJECT

	public:
		StoreyPresenter(StoreyModel* model,
						StoreyView* view,
						QObject* parent);
		virtual ~StoreyPresenter();

		int id() const;

	signals:
		void submitCommand(iscore::SerializableCommand*);
		void elementSelected(QObject*);

		void askUpdate();

	public slots:
		void on_processViewModelCreated(int processId);
		void on_processViewModelDeleted(int processId);

		void on_bottomHandleSelected();
		void on_bottomHandleChanged(int newHeight);
		void on_bottomHandleReleased();

	private:
		void on_processViewModelCreated_impl(ProcessViewModelInterface*);

		StoreyModel* m_model;
		StoreyView* m_view;
		std::vector<ProcessPresenterInterface*> m_processes;

		// Maybe move this out of the state of the presenter ?
		int m_currentResizingValue{}; // Used when the storeyView is being resized.
};

