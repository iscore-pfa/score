#include "ProcessModel.hpp"
#include "LayerModel.hpp"


ProcessModel::ProcessModel(const TimeValue& duration, const id_type<ProcessModel>& id, const QString& name, QObject* parent):
    IdentifiedObject<ProcessModel>{id, name, parent},
    m_duration{duration}
{

}


QByteArray ProcessModel::makeViewModelConstructionData() const { return {}; }


LayerModel*ProcessModel::makeViewModel(
        const id_type<LayerModel>& viewModelId,
        const QByteArray& constructionData,
        QObject* parent)
{
    auto pvm = makeViewModel_impl(viewModelId, constructionData, parent);
    addViewModel(pvm);

    return pvm;
}


LayerModel*ProcessModel::loadViewModel(
        const VisitorVariant& v,
        QObject* parent)
{
    auto pvm = loadViewModel_impl(v, parent);
    addViewModel(pvm);

    return pvm;
}

LayerModel*ProcessModel::cloneViewModel(
        const id_type<LayerModel>& newId,
        const LayerModel& source,
        QObject* parent)
{
    auto pvm = cloneViewModel_impl(newId, source, parent);
    addViewModel(pvm);

    return pvm;
}


LayerModel*ProcessModel::makeTemporaryViewModel(
        const id_type<LayerModel>& newId,
        const LayerModel& source,
        QObject* parent)
{
    return cloneViewModel_impl(newId, source, parent);
}



QVector<LayerModel*> ProcessModel::viewModels() const
{
    return m_viewModels;
}


void ProcessModel::expandProcess(ExpandMode mode, const TimeValue& t)
{
    if(mode == ExpandMode::Scale)
    {
        setDurationAndScale(t);
    }
    else if (mode == ExpandMode::Grow)
    {
        if(duration() < t)
            setDurationAndGrow(t);
        else
            setDurationAndShrink(t);
    }
}


void ProcessModel::setDuration(const TimeValue& other)
{
    m_duration = other;
}


const TimeValue&ProcessModel::duration() const
{
    return m_duration;
}


void ProcessModel::addViewModel(LayerModel* m)
{
    connect(m, &LayerModel::destroyed,
            this, [=] () { removeViewModel(m); });
    m_viewModels.push_back(m);
}


void ProcessModel::removeViewModel(LayerModel* m)
{
    int index = m_viewModels.indexOf(m);

    if(index != -1)
    {
        m_viewModels.remove(index);
    }
}


ProcessModel* parentProcess(QObject* obj)
{
    QString objName (obj ? obj->objectName() : "INVALID");
    while(obj && !obj->inherits("ProcessModel"))
    {
        obj = obj->parent();
    }

    if(!obj)
        throw std::runtime_error(
                QString("Object (name: %1) is not child of a Process!")
                .arg(objName)
                .toStdString());

    return static_cast<ProcessModel*>(obj);
}


const ProcessModel* parentProcess(const QObject* obj)
{
    QString objName (obj ? obj->objectName() : "INVALID");
    while(obj && !obj->inherits("ProcessModel"))
    {
        obj = obj->parent();
    }

    if(!obj)
        throw std::runtime_error(
                QString("Object (name: %1) is not child of a Process!")
                .arg(objName)
                .toStdString());

    return static_cast<const ProcessModel*>(obj);
}
