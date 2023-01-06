namespace TwinStar {

	// ------------------------------------------------

	export class ThreadManager {

		private m_pool: Array<{
			thread: Core.Miscellaneous.Thread;
			context: Core.Miscellaneous.Context;
			result: [boolean, any];
		}> = [];

		// ------------------------------------------------

		make_executor(
			index: number,
			executor: () => any,
		) {
			return () => {
				let item = this.m_pool[index];
				item.result = [false, undefined];
				try {
					let result = executor();
					item.result = [false, result];
				} catch (e: any) {
					item.result = [true, e];
				}
			};
		}

		resize(
			size: number,
			initializer: null | (() => any) = null,
		): void {
			this.m_pool = new Array(size);
			for (let index = 0; index < size; ++index) {
				this.m_pool[index] = {
					thread: Core.Miscellaneous.Thread.default(),
					context: Core.Miscellaneous.g_context.spawn(),
					result: [false, undefined],
				};
				if (initializer !== null) {
					this.execute(index, this.make_executor(index, initializer));
				}
			}
			if (initializer !== null) {
				this.wait();
			}
			return;
		}

		idle(
			index: number
		): boolean {
			if (index >= this.m_pool.length) {
				throw new Error(`#${index} : invalid thread index`);
			}
			let item = this.m_pool[index];
			return !item.context.busy().value;
		}

		execute(
			index: number,
			executor: () => any,
		): void {
			if (index >= this.m_pool.length) {
				throw new Error(`#${index} : invalid thread index`);
			}
			let item = this.m_pool[index];
			if (item.context.busy().value) {
				throw new Error(`#${index} : context is busy`);
			}
			item.context.execute(this.make_executor(index, executor), item.thread);
			item.thread.detach();
			return;
		}

		result(
			index: number,
		): [boolean, any] {
			if (index >= this.m_pool.length) {
				throw new Error(`#${index} : invalid thread index`);
			}
			let item = this.m_pool[index];
			if (item.context.busy().value) {
				throw new Error(`#${index} : context is busy`);
			}
			return item.result;
		}

		push_execute(
			executor: () => any,
		): void {
			if (this.m_pool.length === 0) {
				throw new Error(`thread pool is empty`);
			}
			let index = null;
			while (index === null) {
				index = this.m_pool.findIndex((e, i) => (this.idle(i)));
				if (index === -1) {
					index = null;
					Core.Miscellaneous.Thread.yield();
				}
			}
			this.execute(index, executor);
			return;
		}

		wait(
		): void {
			while (!this.m_pool.every((e, i) => (this.idle(i)))) {
				Core.Miscellaneous.Thread.yield();
			}
		}

	}

	// ------------------------------------------------

	export const g_thread_manager = new ThreadManager();

	// ------------------------------------------------

}