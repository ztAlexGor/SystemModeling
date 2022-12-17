#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <map>
#include "Model.h"

void bank_first_way();
void bank_second_way();
void hospital();

void testDistribution(RandGenerator* rand);
void testWorkGroup();
void testTransition();
void testCreatorWithDistribution();
void testTransitionCondition();
void schemeFromLab2();

void task_1_2();
void task_3_4();
void task_5_6();

void lab2();
void lab3();


int main() {
	srand(time(NULL));

	lab2();
	//lab3();
}


void bank_first_way() {
	//create scheme
	Create* c = new Create("CREATOR", new Exponential(0.5));
	Process* p1 = new Process("CASHIER_1", new Gaussian(1, 0.3), 3);
	Process* p2 = new Process("CASHIER_2", new Gaussian(1, 0.3), 3);

	c->addNextElement(p1);
	c->addNextElement(p2);
	c->setTransitionCondition([](std::vector<Element*> list, Task* task)-> Element* {
		int minQueue = INT_MAX;
		Element* bestElement = nullptr;
		for (auto i : list) {
			int currQueue = ((Process*)i)->getQueue().size();
			if (currQueue < minQueue) {
				minQueue = currQueue;
				bestElement = i;
			}
		}
		return bestElement;
	});

	p1->setChangeQueueFunction([&](std::vector<Task*> queue)->int{
			if (queue.size() > p2->getQueueSize() + 1)return int(queue.size()) - 1;
			else return -1;
		});

	p2->setChangeQueueFunction([&](std::vector<Task*> queue)->int {
			if (queue.size() > p1->getQueueSize() + 1)return int(queue.size()) - 1;
			else return -1;
		});


	//init model
	std::vector<Task*>q1;
	q1.push_back(new Task());
	q1.push_back(new Task());
	p1->init(new Task(), q1);

	std::vector<Task*>q2;
	q2.push_back(new Task());
	q2.push_back(new Task());
	p2->init(new Task(), q2);

	c->setTnext(0.1);

	//create model
	std::vector<Element*>list;
	list.push_back(c);
	list.push_back(p1);
	list.push_back(p2);
	Model model(list);

	model.simulate(100.0);
}

void bank_second_way() {
	//create scheme
	Create* c = new Create("CREATOR", new Exponential(0.5));
	Process* p1 = new Process("CASHIER_1", new Gaussian(1, 0.3), 3);
	Process* p2 = new Process("CASHIER_2", new Gaussian(1, 0.3), 3);

	WorkGroup* wg = new WorkGroup(std::vector<Process*>{p1, p2}, 0);
	wg->setName("CASSIERS");

	//c->setTransitionCondition([](std::vector<Element*> list, Task* task)-> Element* {
	//	int minQueue = INT_MAX;
	//	Element* bestElement = nullptr;
	//	for (auto i : list) {
	//		int currQueue = ((Process*)i)->getQueue().size();
	//		if (currQueue < minQueue) {
	//			minQueue = currQueue;
	//			bestElement = i;
	//		}
	//	}
	//	return bestElement;
	//	});
	
	wg->setChangeQueueFunction([](std::vector<Process*> processes, std::vector<Task*> queue)->int {
			for (int p = 0; p < processes.size(); p++) {
				if (processes[p]->getQueueSize() + 1 < queue.size())return int(queue.size()) - 1;
				else return -1;
			}
		});

	//init model
	std::vector<Task*>q1;
	q1.push_back(new Task());
	q1.push_back(new Task());
	p1->init(new Task(), q1);

	std::vector<Task*>q2;
	q2.push_back(new Task());
	q2.push_back(new Task());
	p2->init(new Task(), q2);

	c->setTnext(0.1);

	//create model
	c->addNextElement(wg);

	std::vector<Element*>list;
	list.push_back(c);
	list.push_back(wg);
	Model model(list);

	model.simulate(50.0);
}

void hospital() {
	//patient generator
	std::vector<std::pair<Task*, double>> distribution = { std::pair<Task*, double>(new Task("1"), 0.5),
															std::pair<Task*, double>(new Task("2"), 0.1),
															std::pair<Task*, double>(new Task("3"), 0.4) };
	Create* creator = new Create("CREATOR", new Exponential(15), distribution);


	//admission department
	auto delayFunction = [](Task* task)->double {
		if (task->getType() == "1")return Exponential{ 15.0 }.generate();
		if (task->getType() == "2")return Exponential{ 40.0 }.generate();
		if (task->getType() == "3")return Exponential{ 30.0 }.generate();
		throw "Unknown task type";
		};

	Process* doctor1 = new Process("DOCTOR1", new CustomGenerator(delayFunction), 0);
	Process* doctor2 = new Process("DOCTOR2", new CustomGenerator(delayFunction), 0);

	//int (*doctorSelectFunc)(std::vector<Task*>) = [](std::vector<Task*> queue) {
	//	for (int i = 0; i < queue.size(); i++) {
	//		if (queue[i]->getType() == "1")return i;
	//	}
	//	return 0;
	//};
	// 
	//doctor1->setSelectTaskFunction(doctorSelectFunc);
	//doctor2->setSelectTaskFunction(doctorSelectFunc);

	Element* (*transitionConditionForDoctors) (std::vector<Element*>, Task*) = [](std::vector<Element*>list, Task* task) {
		if (task->getType() == "1") {
			for (auto e : list) {
				if (e->getName() == "GUIDES")return e;
			}
		}
		else {
			for (auto e : list) {
				if (e->getName() == "WAY_TO_LAB")return e;
			}
		}
	};

	doctor1->setTransitionCondition(transitionConditionForDoctors);
	doctor2->setTransitionCondition(transitionConditionForDoctors);

	std::vector<Process*> doctorsList;
	doctorsList.push_back(doctor1);
	doctorsList.push_back(doctor2);

	WorkGroup* doctors = new WorkGroup(doctorsList, INT_MAX);
	doctors->setName("DOCTORS");
	doctors->setSelectTaskFunction([](std::vector<Task*> queue)->int {
			for (int i = 0; i < queue.size(); i++) {
				if (queue[i]->getType() == "1")return i;
			}
			return 0;
		});

	//guides for patient 1
	Process* guide1 = new Process("GUIDE1", new Uniform(3, 8), 0);
	Process* guide2 = new Process("GUIDE2", new Uniform(3, 8), 0);
	Process* guide3 = new Process("GUIDE3", new Uniform(3, 8), 0);

	std::vector<Process*> guidesList;
	guidesList.push_back(guide1);
	guidesList.push_back(guide2);
	guidesList.push_back(guide3);

	WorkGroup* guides = new WorkGroup(guidesList);
	guides->setName("GUIDES");

	//laboratory way
	Transition* labWay = new Transition("WAY_TO_LAB", new Uniform(2, 5));
	Transition* admissionWay = new Transition("WAY_TO_ADMISSION", new Uniform(2, 5));

	//laboratory
	Process* labRegistry = new Process("REGISTRY", new Erlang(4.5, 3));
	
	Process* laborant1 = new Process("LABORANT1", new Erlang(4, 2), 0);
	Process* laborant2 = new Process("LABORANT2", new Erlang(4, 2), 0);

	Element* (*transitionConditionForLaborants) (std::vector<Element*>, Task*) = [](std::vector<Element*>list, Task* task)->Element* {
		if (task->getType() == "2") {
			task->setType("1");
			return list[0];
		}
		else {
			return nullptr;
		}
	};

	laborant1->setTransitionCondition(transitionConditionForLaborants);
	laborant2->setTransitionCondition(transitionConditionForLaborants);

	std::vector<Process*> laborantList;
	laborantList.push_back(laborant1);
	laborantList.push_back(laborant2);

	WorkGroup* laborants = new WorkGroup(laborantList);
	laborants->setName("LABORANTS");

	//create a scheme
	creator->addNextElement(doctors);

	doctors->addNextElement(guides);
	doctors->addNextElement(labWay);
	
	labWay->addNextElement(labRegistry);
	labRegistry->addNextElement(laborants);
	laborants->addNextElement(admissionWay);
	admissionWay->addNextElement(doctors);
	
	std::vector<Element*>list;
	list.push_back(creator);
	list.push_back(doctors);
	list.push_back(guides);
	list.push_back(labWay);
	list.push_back(labRegistry);
	list.push_back(laborants);
	list.push_back(admissionWay);
	
	Model model(list);

	model.simulate(100.0);
}

void testWorkGroup() {
	Create* c = new Create("CREATOR", new Uniform(1, 1));

	std::vector<Process*>p;
	p.push_back(new Process("p1", new Exponential(0.3), 0));
	p.push_back(new Process("p2", new Exponential(0.5), 0));

	WorkGroup* w = new WorkGroup(p, 5/*"WorkGroup",*/);

	c->addNextElement(w);

	std::vector<Element*>el;
	el.push_back(c);
	el.push_back(w);
	Model model(el);
	model.simulate(30);
}

void testTransition()
{
	Create* c = new Create("CREATOR", new Uniform(1, 1));
	Transition* t = new Transition("TRANSITION", new Exponential(0.4));

	c->addNextElement(t);

	std::vector<Element*>list;
	list.push_back(c);
	list.push_back(t);
	Model model(list);

	model.simulate(20.0);
}

void testDistribution(RandGenerator* rand = new Erlang(4, 2)) {
	//std::vector<double> v;
	//for (int i = 0; i < 50; i++) {
	//	v.push_back(rand->generate());
	//}
	//std::sort(v.begin(), v.end());

	std::map<int, int> hist;
	for (int n = 0; n < 10000; ++n) {
		++hist[2 * rand->generate()];
	}
	for (auto p : hist) {
		if (p.second / 100. > 0.5)
			std::cout
			<< std::fixed << std::setprecision(1)
			<< p.first / 2.0 << '-' << (p.first + 1) / 2.0 << ' '
			<< std::string(p.second / 100, '*') << '\n';
	}
}

void testCreatorWithDistribution() {
	std::vector<std::pair<Task*, double>> distribution = {	std::pair<Task*, double>(new Task("1"), 0.5),
															std::pair<Task*, double>(new Task("2"), 0.1),
															std::pair<Task*, double>(new Task("3"), 0.4) };

	Create* c = new Create("CREATOR", new Uniform(1,1), distribution);
	
	int a[3] ={0, 0, 0};
	for (size_t i = 0; i < 10000; i++){
		Task* t = nullptr;//c->createNewTask();
		if (t->getType() == "1")a[0]++;
		else if (t->getType() == "2")a[1]++;
		else if (t->getType() == "3")a[2]++;
	}
	std::cout << a[0] << std::endl<< a[1] << std::endl<< a[2] << std::endl;
}

void testTransitionCondition() {
	Create* c = new Create("CREATOR", new Uniform(2, 3), new Task("1"));
	Process* p1 = new Process("PROCESSOR1", new Uniform(1, 2));
	Process* p2 = new Process("PROCESSOR2", new Exponential(0.5), 3);
	Process* p3 = new Process("PROCESSOR3", new Exponential(0.5), 3);

	c->addNextElement(p1);
	p1->addNextElement(p2);
	p1->addNextElement(p3);
	p1->setTransitionCondition([](std::vector<Element*> list, Task* task) {
		for (auto e : list) {
			if (e->getState() == Element::States::IDLE)return e;
		}
		int min = list[0]->getInfo().quantity, minInd = 0;
		for (int i = 1; i < list.size(); i++) {
			if (list[i]->getInfo().quantity < min) {
				min = list[i]->getInfo().quantity;
				minInd = i;
			}
		}
		return list[minInd];
		});
	std::vector<Element*>list;
	list.push_back(c);
	list.push_back(p1);
	list.push_back(p2);
	list.push_back(p3);
	Model model(list);

	model.simulate(100.0);
}

void schemeFromLab2() {
	Create* c1 = new Create("CREATOR", new Uniform(2, 5));
	Process* p1 = new Process();
	Process* p2 = new Process();
	Process* p3 = new Process();

	p1->setMaxqueue(5);
	p2->setMaxqueue(5);
	p3->setMaxqueue(5);

	p1->setName("PROCESSOR1");
	p2->setName("PROCESSOR2");
	p3->setName("PROCESSOR3");

	p1->setDelayGenerator(new Exponential(5));
	p2->setDelayGenerator(new Exponential(5));
	p3->setDelayGenerator(new Exponential(5));

	c1->addNextElement(p1);
	p1->addNextElement(p2);
	p2->addNextElement(p3);

	std::vector<Element*>list;
	list.push_back(c1);
	list.push_back(p1);
	list.push_back(p2);
	list.push_back(p3);
	Model model(list);

	model.simulate(100.0);
}




void task_1_2()
{
	Create* c = new Create("CREATOR", new Exponential(5));
	Process* p = new Process("PROCESSOR", new Exponential(5), 5);

	c->addNextElement(p);

	Model model(std::vector<Element*>{c, p});
	model.simulate(1000.0);
}

void task_3_4() {
	Create* c1 = new Create("CREATOR", new Exponential(4));
	Process* p1 = new Process("PROCESSOR1", new Exponential(4));
	Process* p2 = new Process("PROCESSOR2", new Exponential(3));
	Process* p3 = new Process("PROCESSOR3", new Exponential(7));

	p1->setMaxqueue(5);
	p2->setMaxqueue(0);
	p3->setMaxqueue(8);

	c1->addNextElement(p1);
	p1->addNextElement(p2);
	p2->addNextElement(p3);

	std::vector<Element*>list;
	list.push_back(c1);
	list.push_back(p1);
	list.push_back(p2);
	list.push_back(p3);
	Model model(list);

	model.simulate(2000.0);
}


void task_5_6() {
	Create* c = new Create("CREATOR", new Exponential(3));

	std::vector<Process*>p;
	p.push_back(new Process("p1", new Exponential(5), 0));
	p.push_back(new Process("p2", new Exponential(5), 0));

	WorkGroup* w = new WorkGroup(p, 5);

	c->addNextElement(w);

	std::vector<Element*>el;
	el.push_back(c);
	el.push_back(w);
	Model model(el);
	model.simulate(1000);
}

void lab2()
{
	//task_1_2();
	task_3_4();
	//task_5_6();
}

void lab3()
{
	std::cout << "Press 1 to choose the bank task and 2 to choose hospital" << std::endl;
	int choice = _getch() - 48;

	if (choice == 1) {
		bank_second_way();
		//bank_first_way();
	}
	else if (choice == 2) {
		hospital();
	}
	//testWorkGroup();
	//test();
	//hospital();
	//testTransition();
}
