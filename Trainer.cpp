#include "Trainer.h"
#include <io.h>

Trainer::Trainer()
{

}

Trainer::Trainer(Mat& background, const char* folderPath, const char* fileType)
{
	std::vector<String> testImgs = VaildFileList( getAllFile(folderPath, fileType) );

	size_t length = testImgs.size();
	fprintf_s(stdout, "发现[%llu]个可读取图片文件.\n", length);

	for (size_t i = 0; i < length; i++)
	{
		Mat tImg = imread(testImgs[i]);
		CVApp tho(tImg, background);
		PushFeatureData(*tho.getTargets(), testImgs[i]);
	}
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
		if (it->GetTargetState() == TargetState::TS_Normal)
		{
			feature_Data.AddNewData(it->TFea.TFea, fileName);
		}
		else if (it->GetTargetState() == TargetState::TS_OutImage) {
			fprintf_s(stdout, "跨边界物体，已略去.\n\n");
		}
	}
}

void Trainer::SaveTClassifier(const char* Target_Name)
{
	target_Name = String(Target_Name);

	Analysis();
	target_Class.SaveClassifier();
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
		exit(-1);
	}

	fprintf_s(fp, "面积\t周长\t最长轴\t最短轴\t角点数\t矩形度\t圆形度\t偏心率\n");

	auto it = feaIn.begin();
	auto it_end = feaIn.end();
	for (; it != it_end; it++) {
		fprintf_s(fp, "%u\t", it->size);
		fprintf_s(fp, "%d\t", it->Peripheral);
		fprintf_s(fp, "%d\t", it->MER.width);
		fprintf_s(fp, "%d\t", it->MER.height);
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
	if (feature_Data.IsEmpty()) {
		return;
	}

	Sieve_1(0.30);
	Sieve_2(0.20);
	Sieve_2(0.15);
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
		SaveData_d(String(DESKTOP).append(StrAddChar(target_Name, i).c_str()), featrue_List[i]);
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
			feature_sum.Peripheral += feas[i].Peripheral;
			feature_sum.MER.width += feas[i].MER.width;
			feature_sum.MER.height += feas[i].MER.height;
			feature_sum.corners += feas[i].corners;
			feature_sum.Rectangularity += feas[i].Rectangularity;
			feature_sum.consistency += feas[i].consistency;
			feature_sum.eccentricity += feas[i].eccentricity;
		}

		feature_sum.size = (uint)(feature_sum.size / (double)sum);
		feature_sum.Peripheral = (uint)(feature_sum.Peripheral / (double)sum);
		feature_sum.MER.width = (uint)(feature_sum.MER.width / (double)sum);
		feature_sum.MER.height = (uint)(feature_sum.MER.height / (double)sum);
		feature_sum.corners = (uint)(feature_sum.corners / (double)sum);
		feature_sum.Rectangularity /= (double)sum;
		feature_sum.consistency /= (double)sum;
		feature_sum.eccentricity /= (double)sum;
	}

	return feature_sum;
}
