"""Network base class"""
import os
import pickle
from abc import ABC, abstractmethod

"""In Pytorch you would usually define the `forward` function which performs all the interesting computations"""


class Network(ABC):
    """
    Abstract Dataset Base Class
    All subclasses must define forward() method
    """

    def __init__(self, model_name='dummy_network'):
        """
        :param model_name: A descriptive name of the model
        """
        self.model_name = model_name

    @abstractmethod#使用 @abstractmethod 的主要目的是确保某些方法在子类中必须被实现，从而保证一定的接口一致性。
    def forward(self, X):
        """perform the forward pass through a network"""

    def __repr__(self):
        return "This is the base class for all networks we will use"

    @abstractmethod
    def save_model(self, data=None):
        """ each model should know what are the relevant things it needs for saving itself."""


class Dummy(Network):
    """
    Dummy machine
    """

    def __init__(self, model_name="dummy_machine"):
        """
        :param model_name: A descriptive name of the model
        """
        super().__init__()
        self.model_name = model_name

    def forward(self, x):
        """
        :param x: The input to the network
        :return: set x to any integer larger than 59 to get passed
        """

        ########################################################################
        # TODO                                                                 #
        # Implement the dummy machine function.                                #
        #                                                                      #
        ########################################################################

        x = 60


        ########################################################################
        #                           END OF YOUR CODE                           #
        ########################################################################

        return x

    def __repr__(self):
        return "A dummy machine"

    def save_model(self, data=None):
        directory = 'models'
        model = {self.model_name: self}
        if not os.path.exists(directory):
            os.makedirs(directory)
        pickle.dump(model, open(directory + '/' + self.model_name + '.p', 'wb'))