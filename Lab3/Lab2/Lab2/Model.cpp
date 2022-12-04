#include "Model.h"


Model::Model(const std::vector<Element*>& elements) {
	list = elements;
	tnext = tcurr = 0.0;
	event = nullptr;
}

void Model::simulate(double time) {
	while (tcurr <= time) {
		tnext = DBL_MAX;
		for (Element* e : list) {
			if (e->getTnext() < tnext) {
				tnext = e->getTnext();
				event = e;
			}
		}
		if (tnext > time)break;

		//for (Element* e : list) {
		//	e->doStatistics(tnext - tcurr);
		//}
		tcurr = tnext;
		for (Element* e : list) {
			e->setTcurr(tcurr);
		}
		for (Element* e : list) {
			if (e->getTnext() == tcurr) {
				e->outAct();
			}
		}

		std::cout << std::fixed << std::setprecision(1)
			<< "It's time for event in " << event->getName() << ", time = " << tcurr << std::endl;
		printInfo();

		bool queueChanged = false;
		for (Element* e : list) {
			queueChanged = e->checkChanges() || queueChanged;
		}
		if (queueChanged)printInfo();
	}
	printResult();
}

void Model::printInfo() {
	for (Element* e : list) {
		e->printInfo();
	}
	std::cout << std::endl << std::endl << std::endl;
}

void Model::printResult() {
	std::cout << "\n\n\n\t**************************STATISTICS FOR EACH ELEMENT**************************\n\n";
	for (Element* e : list) {
		e->printResult(tcurr);
		std::cout << std::endl << std::endl << std::endl;
	}

	Info statistics = CollectInfo();
	std::cout << "\n\n\n\t********************************TOTAL STATISTICS*******************************\n\n";
	//statistics.printTotalResults();	Extended
	statistics.print(Info::QUANTITY | Info::WORKLOAD | Info::TIME_IN | Info::TIME_OUT | Info::QUEUE_SZ | Info::FAILURES | Info::QUEUE_CH | Info::LIFETIME, 28);
	std::cout << std::endl << std::endl << std::endl;
}




Info Model::CollectInfo() {
	Info totalStats;

	int processCount = 0;
	for (Element* e : list) {
		if (dynamic_cast<Process*>(e) || dynamic_cast<WorkGroup*>(e))
		{
			processCount++;
			totalStats.accumulateInfo(e->getInfo());
		}
	}

	totalStats.meanQueueSize *= processCount;
	totalStats.stopModeling(processCount);
	/*totalStats.meanWorkload /= processCount;
	totalStats.meanQueueSize /= processCount;
	totalStats.meanTimeIntervalBetweenTasks /= totalStats.quantity;
	totalStats.failure /= (totalStats.quantity + totalStats.failure)/100.0;
	totalStats.meanTaskLifetime /= totalStats.quantity;*/
	return totalStats;
}
