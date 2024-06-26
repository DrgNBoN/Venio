#include "Model.hxx"

Model::Model(LossFunction *loss_function, const std::vector<std::shared_ptr<Layer>> &layers)
{
    _model_size = 0;
    for (const auto i : layers)
    {
        addLayer(i);
    }

    /*std::cout << "Network size = " << _model_size;
    system("pause");
    exit(0);

        Be accurate it dangerous!!!!
        */

    _loss_function = loss_function;
}
Model::~Model()
{
    _layers.clear();
    delete _loss_function;
}

void Model::addLayer(std::shared_ptr<Layer> layer)
{

    if (_layers.empty())
    {
        ++_model_size;
        _layers.push_back(layer);
        _layers[0]->buildFirstLayer();
        return;
    }
    ++_model_size;
    _layers.push_back(layer);
    _layers[_layers.size() - 1]->buildLayer(_layers[_layers.size() - 2]->getLayerSize());
}

void Model::Log()
{
    std::cout << "Input size : " << _layers[0]->getLayerSize() << ", Input: " << getInput() << "\n";

    printf("Number of layers: %zu \n", _layers.size());

    std::cout << "Layers: \n";

    int k = 1;

    for (const auto &i : _layers)
    {
        printf("\tLayer num: %d,Layer size: %d,Layer active values: ", k++, i->getLayerSize());
        std::cout << i->getLayerActiveValues() << "\n";
        std::cout << "\tLayer derivations: " << i->getLayerDerivation() << "\n\n";
    }
    std::cout << "Output size : " << _layers[_model_size - 1]->getLayerSize() << ", Output: " << _layers[_model_size - 1]->getLayerActiveValues() << "\n";
}

void Model::forwardPropogation()
{
    for (int i = 1; i < _layers.size(); ++i)
    {
        _layers[i]->propogateLayer(_layers[i - 1]->getLayerActiveValues());
    }

    // Add activation to output (maybe softmax)-----------------
}
void Model::backPropogation(Matrixd right_answer)
{

    _last_right_answer = right_answer;

    Matrixd dt, dw, dh, df, db;
    Matrixd dx = getDerivationLossForLastLayer(std::move(right_answer));

    for (int i = _model_size - 1; i >= 1; --i)
    {

        dh = dx;
        df = _layers[i]->getLayerDerivationMatrix();

        dt = dh.array() * df.array();

        dw = _layers[i - 1]->getLayerActiveValues().transpose() * dt; //

        dx = dt * _layers[i]->getLayerWeights().transpose();

        db = dt;

        _layers[i]->setLayerDerivation(dt);
        _layers[i]->setLayerWeightsGradient(dw);
        _layers[i]->setLayerBiasGradient(db);
    }
    // Do work------------------------------------------------------------------
}

void Model::backPropogation(){

    Matrixd dt, dw, dh, df, db, dx;

    try{
        dx = getDerivationLossForLastLayer(_last_right_answer);
    }
    catch(std::exception e){
        std::cout << "Warning _last_right_answer not detected\n" << e.what() << "\n";
        system("pause");
        exit(0);
    }

    
    

    for (int i = _model_size - 1; i >= 1; --i)
    {

        dh = dx;
        df = _layers[i]->getLayerDerivationMatrix();

        dt = dh.array() * df.array();

        dw = _layers[i - 1]->getLayerActiveValues().transpose() * dt; //

        dx = dt * _layers[i]->getLayerWeights().transpose();

        db = dt;

        _layers[i]->setLayerDerivation(dt);
        _layers[i]->setLayerWeightsGradient(dw);
        _layers[i]->setLayerBiasGradient(db);
    }
}

// getters & setters for class Model---------------------------------------------------------------------
void Model::setInput(Matrixd input)
{
    _layers[0]->setLayerActiveValues(std::move(input));
}
void Model::setLayerDerivation(size_t number_of_layer, Matrixd new_derivation_neurons_matrix)
{
    _layers[number_of_layer]->setLayerDerivation(std::move(new_derivation_neurons_matrix));
}
void Model::setLayerBias(size_t number_of_layer, Matrixd new_bias_matrix)
{
    _layers[number_of_layer]->setLayerBias(std::move(new_bias_matrix));
}
void Model::setLayerValues(size_t number_of_layer, Matrixd new_values_matrix)
{
    _layers[number_of_layer]->setLayerValues(std::move(new_values_matrix));
}
void Model::setLayerActiveValues(size_t number_of_layer, Matrixd new_active_values_matrix)
{
    _layers[number_of_layer]->setLayerActiveValues(std::move(new_active_values_matrix));
}
void Model::setLayerWeights(size_t number_of_layer, Matrixd new_weights_matrix)
{
    _layers[number_of_layer]->setLayerWeights(std::move(new_weights_matrix));
}
void Model::setLayerActivationFunction(size_t number_of_layer, ActivationFunction *new_activation_function)
{

    //Warning---------------------------------------
    _layers[number_of_layer]->setLayerActivationFunction(new_activation_function);
}
void Model::setModelLossFunction(LossFunction *new_loss_function)
{
    //Warning---------------------------------------
    _loss_function = new_loss_function;
}

Matrixd Model::getDerivationLossForLastLayer(Matrixd right_answer)
{
    return _loss_function->getDerivationLoss(_layers[_layers.size() - 1]->getLayerActiveValues(), std::move(right_answer));
}

Matrixd Model::getLayerBias(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerBias();
}
Matrixd Model::getLayerValues(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerValues();
}
Matrixd Model::getLayerActiveValues(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerActiveValues();
}
Matrixd Model::getLayerWeights(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerWeights();
}
Matrixd Model::getLayerBiasGradient(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerBiasGradient();
}
Matrixd Model::getLayerWeightsGradient(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerWeightsGradient();
}
Matrixd Model::getLayerDerivation(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerDerivation();
}
ActivationFunction *Model::getLayerActivationFunction(size_t number_of_layer)
{
    return _layers[number_of_layer]->getLayerActivationFunction();
}
LossFunction *Model::getModelLossFunction()
{
    return _loss_function;
}
Matrixd Model::getOutput()
{
    return _layers[_model_size - 1]->getLayerActiveValues();
}
Matrixd Model::getInput()
{
    return _layers[0]->getLayerActiveValues();
}
size_t Model::getLayersSize()
{
    return _layers.size();
}
double Model::getAverageLoss(const Matrixd& right_answer)
{
    return _loss_function->getMediumLoss(_layers[getLayersSize()-1]->getLayerActiveValues(),right_answer);
}

// getters & setters for class Model---------------------------------------------------------------------
