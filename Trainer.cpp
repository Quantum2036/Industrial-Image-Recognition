#include "Trainer.h"
#include <io.h>
#include <thread>
#include <mutex>

Trainer::Trainer()
{
	target_Name = String("unnamed");
}

Trainer::Trainer(Mat& background, const char* folderPath, const char* fileType)
{
	target_Name = String("unnamed");
	std::vector<String> testImgs = VaildFileList( getAllFile(folderPath, fileType) );
	size_t length = testImgs.size();	//待扫描的图像数目

	fprintf_s(stdout, "在目录 %s 发现[%llu]个可读取图片文件.\n", folderPath, length);
	fprintf_s(stdout, "扫描中...\t");

	TickMeter timer;	//计时器

	timer.start();
	threadProcess_8(testImgs, background);
	timer.stop();

	fprintf_s(stdout, "完成\t平均扫描用时 %.2lf ms\t", timer.getTimeMilli() / length);
	fprintf_s(stdout, "捕获 有效目标 [%llu]\n", feature_Data.getSize());
}

String Trainer::StrAddChar(const String& str, size_t length)
{
	String name = str;
	name.append("_");
	if (length < 26) {
		String s;
		char c = 65 + (char)length;
		s = c;
		name += s;
	}
	else {
		String number = std::to_string((long double)length);
		name += number;
	}
	return name;
}

void Trainer::PushFeatureData(std::vector<Target>& target, String fileName)
{
	auto it = target.begin();
	auto it_end = target.end();

	for (; it != it_end; it++) {
		if (it->GetTargetState() == TargetState::TS_Normal) {
			feature_Data.AddNewData(it->TFea.Struct_feature, fileName);
		}
	}
}

void Trainer::SaveTClassifier(const char* Target_Name, const char* SavePath)
{
	target_Name = String(Target_Name);

	Analysis();

	target_Class.SaveClassifier(SavePath);
}

void Trainer::threadProcess_8(std::vector<String> image_names, Mat& background)
{
	size_t sum = image_names.size();
	size_t avg = sum / 8;

	size_t num_0 = 0, num_1 = num_0 + avg, num_2 = num_1 + avg, num_3 = num_2 + avg;
	size_t num_4 = num_3 + avg, num_5 = num_4 + avg, num_6 = num_5 + avg, num_7 = num_6 + avg, num_8 = sum;

	std::mutex mu;
	auto Sub_fun = [&](size_t begin, size_t end) {
		for (size_t i = begin; i < end; i++)
		{
			Mat tImg = imread(image_names[i]);
			CVApp tho(tImg, background);

			mu.lock();
			PushFeatureData(*tho.getTargets(), image_names[i]);
			mu.unlock();
		}
	};

	std::thread thread_01(Sub_fun, num_0, num_1);
	std::thread thread_02(Sub_fun, num_1, num_2);
	std::thread thread_03(Sub_fun, num_2, num_3);
	std::thread thread_04(Sub_fun, num_3, num_4);
	std::thread thread_05(Sub_fun, num_4, num_5);
	std::thread thread_06(Sub_fun, num_5, num_6);
	std::thread thread_07(Sub_fun, num_6, num_7);
	std::thread thread_08(Sub_fun, num_7, num_8);

	thread_01.join();
	thread_02.join();
	thread_03.join();
	thread_04.join();
	thread_05.join();
	thread_06.join();
	thread_07.join();
	thread_08.join();
}

void Trainer::SaveData_d(String& Save_Full_Path, std::vector<feature>& feaIn)
{
	if (Save_Full_Path.size() -  Save_Full_Path.rfind(".txt") != 4) {
		Save_Full_Path.append(".txt");
	}

	FILE* fp = nullptr;
	fopen_s(&fp, Save_Full_Path.c_str(), "w");
	if (!fp) {
		fprintf_s(stderr, "ERROR: Open file fail. \n");
		exit(EXIT_FAILURE);
	}

	fprintf_s(fp, "面积\t周长\t最长轴\t最短轴\t中空\t角点数\t矩形度\t圆形度\t偏心率\n");

	auto it = feaIn.begin();
	auto it_end = feaIn.end();
	for (; it != it_end; it++) {
		fprintf_s(fp, "%u\t", it->size);
		fprintf_s(fp, "%d\t", it->peripheral);
		fprintf_s(fp, "%d\t", it->major_axis);
		fprintf_s(fp, "%d\t", it->minor_axis);
		fprintf_s(fp, "%d\t", it->isHollow);
		fprintf_s(fp, "%d\t", it->corners);
		fprintf_s(fp, "%.3lf\t", it->Rectangularity);
		fprintf_s(fp, "%.3lf\t", it->consistency);
		fprintf_s(fp, "%.3lf\n", it->eccentricity);
	}

	fclose(fp);
}

std::vector<String> Trainer::getAllFile(String folderPath, String type)
{
	std::vector<String> files;

	// 文件句柄
	intptr_t hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	String p;

	if ((hFile = _findfirst(p.assign(folderPath).append("\\*" + type).c_str(), &fileinfo)) != -1) {
		do {
			// 保存文件的全路径
			files.push_back(p.assign(folderPath).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}

	return files;
}

std::vector<String> Trainer::VaildFileList(std::vector<String> fileList)
{
	size_t length = fileList.size();
	std::vector<String> vaildFile;

	for (size_t i = 0; i < length; i++)
	{
		if (imread(fileList[i]).data != nullptr) {
			vaildFile.emplace_back(fileList[i]);
		}
	}
	fileList.clear();

	return vaildFile;
}

void Trainer::Analysis(void)
{
	fprintf_s(stdout, "分析中...\t");

	if (feature_Data.IsEmpty()) {
		fprintf_s(stdout, "未找到数据\n");
		return;
	}
	
	Sieve_1(0.30);
	Sieve_2(0.20);
	Sieve_2(0.15);

	fprintf_s(stdout, "完成\n\n");
}

void Trainer::Sieve_1(double errlimit)
{
	std::vector<TClassifier> feature_List;

	for (size_t i = 0; i < feature_Data.getSize(); i++)
	{
		feature temp_feature = feature_Data.getFeatureAt(i);
		String temp_name = feature_Data.getNameAt(i);

		for (size_t j = 0; j < feature_List.size(); j++)
		{
			//若在某个类别中发现相似，则将特征载入到类别后跳出循环
			if (feature_List[j].IsExistTFeature(temp_feature, errlimit))
			{
				feature_List[j].AddNewData(temp_feature, temp_name);
				goto TAG_NEXTLOOP_Sieve_1;
			}
		}
		
		//仅当发现新特征时被执行
		feature_List.emplace_back();
		feature_List[feature_List.size() - 1].AddNewData(temp_feature, temp_name);

	//	进入下一个外层循环为下一个*it分类
	TAG_NEXTLOOP_Sieve_1:;
	}

	size_t len = feature_List.size();
	for (size_t i = 0; i < len; i++) {
		target_Class.AddNewClass(getAverageFeature(feature_List[i].getDataVector()), StrAddChar(target_Name, i));
	}
}

void Trainer::Sieve_2(double errlimit)
{
	size_t count = target_Class.getSize();
	using vFeature = std::vector<feature>;
	vFeature err_List;
	auto featrue_List = new vFeature[count];

	auto it = feature_Data.getDataBegin();
	auto it_end = feature_Data.getDataEnd();
	for (; it != it_end; it++) {

		for (size_t i = 0; i < count; i++)
		{
			//若在某个类别中发现相似，则将特征载入到类别后跳出循环
			if (TClassifier::IsFeatureEqual(*it, target_Class.getFeatureAt(i), errlimit)) {
				featrue_List[i].emplace_back(*it);
				goto TAG_NEXTLOOP_Sieve_2;
			}
		}

		//仅当发现不匹配特征时被执行
		err_List.emplace_back(*it);

		//	进入下一个外层循环为下一个*it分类
	TAG_NEXTLOOP_Sieve_2:;
	}

	target_Class.clear();
	for (size_t i = 0; i < count; i++) {
		SaveData_d(String(LOGFOLDER).append(StrAddChar(target_Name, i).c_str()), featrue_List[i]);
		target_Class.AddNewClass(getAverageFeature(featrue_List[i]), StrAddChar(target_Name, i));
	}

	delete[] featrue_List;
}

feature Trainer::getAverageFeature(const std::vector<feature>& feas)
{
	size_t sum = feas.size();
	feature feature_sum;
	if (sum) {
		for (size_t i = 0; i < sum; i++)
		{
			feature_sum.size += feas[i].size;
			feature_sum.peripheral += feas[i].peripheral;
			feature_sum.major_axis += feas[i].major_axis;
			feature_sum.minor_axis += feas[i].minor_axis;
			feature_sum.isHollow += feas[i].isHollow;
			feature_sum.corners += feas[i].corners;
			feature_sum.Rectangularity += feas[i].Rectangularity;
			feature_sum.consistency += feas[i].consistency;
			feature_sum.eccentricity += feas[i].eccentricity;
		}

		feature_sum.size = (uint)(feature_sum.size / (double)sum);
		feature_sum.peripheral = (uint)(feature_sum.peripheral / (double)sum);
		feature_sum.major_axis = (uint)(feature_sum.major_axis / (double)sum);
		feature_sum.minor_axis = (uint)(feature_sum.minor_axis / (double)sum);
		feature_sum.isHollow = (feature_sum.isHollow / (double)sum) > 0.5 ? 1 : 0;
		feature_sum.corners = (uint)(feature_sum.corners / (double)sum);
		feature_sum.Rectangularity /= (double)sum;
		feature_sum.consistency /= (double)sum;
		feature_sum.eccentricity /= (double)sum;
	}

	return feature_sum;
}
