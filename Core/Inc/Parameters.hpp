#pragma once

#include<main.h>
#include<list>
#include<stdexcept>

//各モーターにつき1つのインスタンスが与えられるため、クラスの実装としては単一のモーターに対応するパラメーターを考える

namespace stepping_md{
	//モータードライバのモードを表す列挙型
	enum class MD_MODE{
		//起動直後やEmergencyスイッチが押されたときのモードです。
		//モーターは一切動作しません
		//ステッピングMDのENA信号はLOWになっています
		DEFAULT,

		//位置制御モード
		POS
	};

	//モーター制御に用いるパラメーターをまとめて定義した構造体
	struct MotorParam{
		//PID制御は用いない

		//MDのモード
		MD_MODE mode;

		//PPR
		float ppr;

		//TARGET
		float target;
	};

	//Parametersに最低限必要な実装
	//基本的にはそのままにしておくこと
	class Parameters_Base{
		private:
			static std::list<Parameters_Base> instances;
		protected:
			explicit Parameters_Base(){instances.push_back(*this);}
		public:
			inline static void trigger_emergency_callback(void){
				for(Parameters_Base controller : instances){
					controller.emergency_callback();
				}
			}
			virtual void emergency_callback(void){throw std::logic_error("emergency_callback is not implemented");}
	};

	//パラメーターの書き込み・読み込みを行うクラス
	//要実装
	class Parameters : public Parameters_Base{
		private:
		        uint16_t BID;
			//Parameters上で扱うMD_MODEとpprとtargetのメンバ変数
			MotorParam SMParam;
		public:
			//コンストラクタ(引数やオーバーロードは自由に決めてよい)
			explicit Parameters(){}

			//パラメータを取得する関数
			//第1引数は結果を格納する構造体オブジェクトへのポインタ
			MotorParam get_motor_param();

			//パラメータを設定する関数
			//第1引数は設定内容が書かれた構造体オブジェクト
			void set_motor_param(const MotorParam& param);

			//BIDを取得する関数
			uint16_t get_BID();

			//BIDを設定する関数
			void set_BID(const uint16_t bid);

			//Emergencyボタンが押されたときに呼ばれるコールバック関数
			//emergency_callbackはパラメーターのうちmodeをMD_MODE::DEFAULTにする関数
			void emergency_callback(void) override
			{
				SMParam.mode = MD_MODE::DEFAULT;
			}
	};
}
