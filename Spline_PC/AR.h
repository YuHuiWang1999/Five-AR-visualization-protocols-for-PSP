#pragma once
#include <Python.h>
#include <QtWidgets/QMainWindow>
#include "ui_AR.h"
#include "Calibration.h"
#include "Globalvar.h"
#include "UQICPRegister.h"
#include "AxesHelper.h"
#include "myvector3.h"
#include "mygroup.h"
#include <QMessageBox>
#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include <QDateTime>
#include <QUdpSocket>

#include <algorithm> // ���� std::find
#include <random>
#include<iostream>
#include <vtkAutoInit.h>

//pcl#include <pcl/point_types.h>#include <pcl/registration/sample_consensus_prerejective.h>#include <pcl/features/normal_3d.h>#include <pcl/features/fpfh.h>#include <pcl/filters/uniform_sampling.h>#include <pcl/visualization/pcl_visualizer.h>#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

//VTK
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h> // ����vtkIdFilter�࣬���ڱ�����ݼ��еĵ�
#include <vtkIdTypeArray.h> // ����vtkIdTypeArray�࣬���ڴ洢���ID
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkAreaPicker.h>
#include <vtkImageData.h>
#include <vtkCylinderSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCommand.h>
#include <vtkCellPicker.h>
#include <vtkLineSource.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkTextWidget.h>
#include <vtkTextActor.h>
#include <vtkPlaneSource.h>
#include <vtkTextRepresentation.h>
#include <vtkRendererCollection.h>
#include <vtkTextProperty.h>
#include <vtkMatrix3x3.h>
#include <vtkPointSource.h>
#include <vtkSphereSource.h>
#include <vtkSTLWriter.h>
#include <vtkOBJWriter.h>
#include <vtkDataObject.h>
#include <vtkLight.h>
#include <vtkOctreePointLocator.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkPointPicker.h>
#include <vtkPNGReader.h>
#include <vtkTexture.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkContourFilter.h>
#include <vtkDelaunay2D.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkButterflySubdivisionFilter.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h> //���㷨����
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkOBBTree.h>
#include <vtkPNGReader.h>
#include <vtkImageActor.h>
#include <QTimer>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkAutoInit.h>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>   
#include "UQICPRegister.h"
#include <vtkNamedColors.h>
#include <vtkJPEGReader.h>


#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataSilhouette.h> // VTK����������
#include <vtkPropPicker.h> // VTK����ѡ����
#include <vtkLineSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkBooleanOperationPolyDataFilter.h> // ���ڲ�������
#include <vtkAppendPolyData.h> // ���ںϲ����������


#include "PickHighlight.h"
//QT�������
#include <QCamera>  
#include <QCameraViewfinder>  
#include <QCameraImageCapture>  
#include <QCameraInfo>

#define SIZE_PIXEL_x        0.083243
#define SIZE_PIXEL_y        0.085686
#define SSR        1  

VTK_MODULE_INIT(vtkRenderingOpenGL2); // ��ʼ��VTK��OpenGL2��Ⱦģ��
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);// ��ʼ��VTK�Ľ�����ʽģ��
VTK_MODULE_INIT(vtkRenderingFreeType); // ��ʼ��VTK��FreeType��Ⱦģ��

struct Point_2d {
	double X, Y;
};

struct Point_3d {
	double X, Y, Z;
};

// Orien ö��Ҳ��Ҫ������
enum Orien {
	X, Y, Z
};

class AR : public QMainWindow
{
	Q_OBJECT

//new-----------------
protected:
	virtual void keyPressEvent(QKeyEvent *ev);
	bool isUS_collect = false;
	bool P_Record_USProbeCali = false;
	bool isUS_Cail = false;
//new-----------------
public:
	AR(QWidget *parent = nullptr);
	~AR();
	void seclet();
	void recieve1();
	//void sendPosi();//ʵʱ����
	void SendPosition();//ʵʱ����	
	void regis_clear();
	void Test();//ʵʱ����
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	void test_clear();
	void cali_clear();
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	void testdate1();
	void testdate2();
	void closeServer();

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

	//
	void Threepointregister(double sourcePoint1[3], double sourcePoint2[3], double sourcePoint3[3],
		double targetPoint1[3], double targetPoint2[3], double targetPoint3[3],
		vtkMatrix4x4* Matrix);
	void Vec_Normalize(double Vec[3], double Normalize[3]);
	void Vec_Add(double a[3], double b[3], double c[3]);
	QPoint rightClikPoint;


	enum NDIData {

		qx,
		qy,
		qz,
		q0,
		tx,
		ty,
		tz
	};

	std::vector<vtkSmartPointer<vtkActor>> Tool_actors;
	std::vector<vtkSmartPointer<vtkActor>> ball_actors;
	//void AR::TrackTool(vector<double> temp, int index, bool tempFlag);

		//class CoordinateActor;
	// ��ǰѡ�е�Actor
	vtkSmartPointer<vtkActor> selActor = vtkSmartPointer<vtkActor>::New();
	// ��λ����С��
	vtkSmartPointer<vtkActor> cenActor = vtkSmartPointer<vtkActor>::New();
	// ��תActor
	vtkSmartPointer<vtkActor> rotX = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> rotY = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> rotZ = vtkSmartPointer<vtkActor>::New();
	// �ƶ�Actor
	vtkSmartPointer<vtkActor> moveX = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> moveY = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> moveZ = vtkSmartPointer<vtkActor>::New();
	// ĩ�˶�λС��
	vtkSmartPointer<vtkActor> moveXEnd = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> moveYEnd = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> moveZEnd = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> LineActor(Point3d p1, Point3d p2, double color[3]);

	vtkSmartPointer<MyGroup> us_data_group = vtkSmartPointer<MyGroup>::New();
	vtkSmartPointer<MyGroup> stl_data_group = vtkSmartPointer<MyGroup>::New();

public slots:

	void on_set_listen_clicked();
	void uploademode();
	void onButtonCloseClicked();
	void send_data_slot();//����
	void setlabel(int L);//�źű�ǩ
	void  Send_startTimer();
	void  Send_stopTimer();
	void  disconnectClient();
	void loadPatientModel();
	void ShowWinMenu(QPoint point, QPoint localPoint);
	void PointCloudRegister();
	void NumSelect(int num_all, int num_part, vector<int> &result);
	void VecSwith(vtkMatrix4x4* Matrix, double in[3], double out[3]);
	void PointSwith(vtkMatrix4x4* Matrix, double in[3], double out[3]);
	//��¼icp��
	void ICP_GetPoint();
	void ICPCouldRecord();
	void NDIDataToMat(vector<double> temp, vtkMatrix4x4* Matrix);
	void UQtoMat(double q0, double qx, double qy, double qz, Matrix3d &Matrix);
	void SetCamera(vtkRenderer* render, double cameraposition[3], double focalposition[3], double viewup[3]);
	//��¼���Ӽ�̽���λ�ò�תΪ�����µ���Ե�λ��
	void coarse_GetPoint();
	void coarse_Resiger();
	void TrackTool(int index, bool tempFlag, vector<double> temp);
	void RenderTool();
	void StartPointClouds(vtkSmartPointer<vtkActor> actor_target, vtkSmartPointer<vtkActor> actor_source, vector<double>  Search_p , vector<double>  Search_v, vtkSmartPointer<vtkMatrix4x4> Matrix_boneToDRO);
	void inputPointClouds();

	//ui
	void cailpage();
	void nextpage();
	void tool00();
	void tool01();
	void tool02();
	void tool03();
	void tool04();
	void settool();
	void Sexselect_man();
	void Sexselect_women();
	void Splineselect_X();
	void Splineselect_L();

	//
	void segmentimage();
	void ImagDelete();




signals:

	void cali();

private:
	Calibration* calibration;
	Ui::ARClass ui;
	QTcpServer* mServer;
	QTcpSocket* Socket1;

	QUdpSocket *mUdpSocket;
	time_t timestamp;
	int n = 0;
	QString str0;
	//int calin = 0;
	//xxxxxxx
	int n1 = 0;
	QString str1;
	int n2 = 0;
	QString str2;
	int buildnumber = 0;
	vector< vector<double> > build_point;


	//ui
	int statenumber = 0;
	bool tool000 = false;
	bool tool001 = false;
	bool tool002 = false;
	bool tool003 = false;
	bool tool004 = false;

	bool Handeactive = false;
	bool Robotactive = false;
	int sendnumber = 0;

	bool startsend = false;
	bool iscail = false;
	bool isCoarseReg = false;
	bool isICPReg = false;
	bool isICPcollection = false;

	//�궨���
	bool P_Record_USPhantomCali = false;
	//ui
	bool isselectmodle = false;

	bool isPick = false;
	bool ismove = false;
	//
	vector<vector<double> > USsegment_points_inWorld;
	vector<vector<double> > USsegment_points_inTemp;
	vector<vector<double> > USsegment_image_Max_points;
	//
	vector<vector<vector<double>>>USsegment_image_points;


	//��ʱ��
	QTimer* holoTimer = new QTimer;
	QTimer* AllTimer = new QTimer;
	double scale = 1;
	std::ofstream OutFiledate;
	std::ofstream OutPointdate;
	vtkSmartPointer<vtkSTLReader> patient_spline_reader = vtkSmartPointer<vtkSTLReader>::New();
	vtkSmartPointer<vtkRenderer> patientRender = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> AxisRender = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> regRender = vtkSmartPointer<vtkRenderer>::New();


	vtkSmartPointer<vtkActor> Man_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Spline_X_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Spline_L_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Spline_Show_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Man_Body_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Line_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> P_Actor = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkActor> NDI_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> patient_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Probe_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Cube_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Bone_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Guide339_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Guide449_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Drill_Actor = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkActor> tibia_Actor = vtkSmartPointer<vtkActor>::New();//�¼�

	vtkSmartPointer<vtkActor> m_ankleinbone = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> m_ankleoutbone = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> m_regbone = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> m_segbone = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> m_cutbone = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> icp_point_actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> CoarseBone_Actor = vtkSmartPointer<vtkActor>::New();
	
	vtkSmartPointer<vtkActor> USdata_Actor = vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkPolyDataMapper> Ball_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	vtkSmartPointer<vtkMatrix4x4> Matrix_boneToFarDRO = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkSmartPointer<vtkMatrix4x4> Matrix_boneToTibiaDRO = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkSmartPointer<vtkMatrix4x4> Matrix_coarse = vtkSmartPointer<vtkMatrix4x4>::New();//����׼����

	vtkSmartPointer<vtkMatrix4x4> Matrix_show = vtkSmartPointer<vtkMatrix4x4>::New();//����׼����

	vtkSmartPointer<vtkMatrix4x4> Matrix_BonetoDRO = vtkSmartPointer<vtkMatrix4x4>::New();//����


	double Search_pos[3];//���ĵ�Search_pos_����
	double Search_vec[3];//������Search_vec_����

	vector< vector<double> > Search_pos_sigle;//���ĵ�Search_pos_����bone
	vector< vector<double> > Search_vec_sigle;//������Search_vec_����bone
	double PresetPoints_mark_0[3];	double PresetPoints_mark_1[3];	double PresetPoints_mark_2[3];

	//�׹ؽ���
	double ankle_inner_point[3] = { 0 };

	//�׹ؽ���ǰ
	double ankle_inner_a[3] = { 0 };
	//�׹ؽ��ں�
	double ankle_inner_p[3] = { 0 };
	//�׹ؽ���
	double ankle_outer_point[3] = { 0 };
	//�׹ؽ���ǰ
	double ankle_outer_a[3] = { 0 };
	//�׹ؽ����
	double ankle_outer_p[3] = { 0 };
	//�洢ǰ�����
	double Mtibia_anter_point[3];
	//�׹ؽ���
	double Mankle_outer_point[3];
	//�׹ؽ���
	vector< vector<double> > GatherPoints_mark;
	//double tibia_near_outer_point[3];

	//�и���׹ؽ���
	double Mankle_cutted_inner[3];
	//�и���׹ؽ���
	double Mankle_cutted_outer[3];
	//�׹ؽ�����
	double Mankle_center_point[3];
	//�׹ؽ�����
	double Mankle_inner_point[3];
	//�ֹ�ϥ�ؽ�����
	double Mtibia_knee_center_point[3];

	vector< vector<double> > process_points1;
	vector<vector<double> > icp_points;
	vector<vector<double> > DRO_points;////////�����DRO�ĵ�

	double t_x[3], t_y[3], t_z[3];

	//��ʼ��patient����
	void initPatientPage();
	int Mark_Number = 0;


	//US���
	vtkSmartPointer<vtkMatrix4x4> Matrix_rotate = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> Matrix_USToTemp = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> Matrix_actorToimage = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkActor> USActor1 = vtkSmartPointer<vtkActor>::New();
	int Imag_Index = 0;
	//��¼����������̬
	vector< vector<double> > tibiaToWorld;
	vector< vector<double> > USToWorld;

	vector<vector<double>> US_points;

	//��¼�궨US-Probe����
	vector< vector<double> > UsCaliPhantomToWorld;
	vector< vector<double> > UsCailProbeToWorld;
	vector< vector<double> > UsCailUSProbeToWorld;
	vtkSmartPointer<vtkCamera> camera_3D = vtkSmartPointer<vtkCamera>::New();
	vtkSmartPointer<vtkActor> USsegment_actor = vtkSmartPointer<vtkActor>::New();

	PyObject* pModule_Cal = NULL;
	PyObject* pFunc_Cal = NULL;
	//us--��׼
	vector < vtkSmartPointer<vtkMatrix4x4>> Matrix_splineToDRO;
	//��Ƶ��
	QCamera* camera;
	QCameraViewfinder* viewfinder;
	QCameraImageCapture* imageCapture;
	void initcamera();
	void changeVideoSource(int index);
	//�洢������豸��Ϣ
	QList<QCameraInfo> CameraInfoList;
	void onImageCaptured(int id, const QImage& preview);
	//US-PY���
	void initpy();
	void Do_Segment(const char * img, int target);
	void initpath();
	void drawPointsOnImage(const char* imgPath, const QString& outputFolder, vector<vector<vector<double>>> allUSsegmentPoints);
	void USPointCloudRegister();
	//US-probe
	void cail_probe(int mode, int group);
	void record_position_1();
	void cali_phantom_1();
	void record_position_2();
	void calio_USprobe_2();
	//plc������׼
	void convertToPCLPointCloud(const std::vector<std::vector<double>>& points);
	void Open_Write_txt();
	void Open_Read_txt();
	void intPickdata();
	void RubberBandPickCallback(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);
	void ResigterUS(vtkSmartPointer<vtkPolyData> polydata_bone, vector<vector<double>> points);
	void ChangeCamera_3D();
	//
	void translatMode();
	void roateMode();
	void targetMouseMove(int x, int y, bool isMousePressed);
	void PickThisModle(int x, int y);
	void RemoveSubset(vtkPolyData* polyData1, vtkPolyData* polyData2);
	//�����ؼ�
	vtkSmartPointer<AxesHelper> targetHelper = vtkSmartPointer<AxesHelper>::New();
	vector<vtkSmartPointer<vtkActor> > targetActors;
	vtkActor *LastPickedActor;
	vtkProperty *LastPickedProperty;
	QAction *L1SelectAction = new QAction("Spline_L1");
	QAction *L2SelectAction = new QAction("Spline_L2");
	QAction *L3SelectAction = new QAction("Spline_L3");
	QAction *L4SelectAction = new QAction("Spline_L4");
	QAction *L5SelectAction = new QAction("Spline_L5");
	QAction *L6SelectAction = new QAction("Spline_L6");
	QMenu RightMenu;
	vtkSmartPointer<PickHighlight> Pick_Area = PickHighlight::New();
	vtkSmartPointer<vtkPolyData>polydata_Us_Input= vtkSmartPointer<vtkPolyData>::New();
	bool PointExists(vtkPoints* points, double p[3]);
	void SelectSplineL1();
	void AreaPickBoolCut(int index, bool isclicked , double color);
	double m[16] =
	{ 0.821423,-0.57001,-0.0188149 ,-29.5955,
	  0.57032,0.820975 ,0.0270988, -238.198,
	 -1.03992e-17, -0.03299, 0.999456, -120.687,
	  0,0,0,1 };
	void PickUsData();

};
